import KratosMultiphysics
import sqlite3
import hashlib
import json
from pathlib import Path
import numpy as np
try:
    import pandas as pd
    from xlsxwriter import Workbook
    missing_pandas = False
    missing_xlsxwriter = False
except ImportError as e:
    if 'pandas' in str(e):
        missing_pandas = True
        missing_xlsxwriter = True
    elif 'xlsxwriter' in str(e):
        missing_xlsxwriter = True
        missing_pandas = False

class RomDatabase(object):

    def __init__(self, general_rom_manager_parameters, rom_training_parameters, hrom_training_parameters, mu_names):
        self.database_name = "rom_database_sqlite3.db" #TODO This should be made user-defined. Use the same for FOM ROM HROM HHROM??


        self.mu_names = mu_names
        # A single list with the name of each parameter to be used (for reference) in the database, e.g. ["alpha", "permeability", "mach"].
        # self.mu_names are expected to match the position and the number of inputs in the lists mu_train, mu_test, mu_run

        self.general_rom_manager_parameters = general_rom_manager_parameters
        self.rom_training_parameters = rom_training_parameters
        self.hrom_training_parameters = hrom_training_parameters
        self.set_up_or_get_data_base()



    def set_up_or_get_data_base(self):
        rom_output_folder_name = self.rom_training_parameters["Parameters"]["rom_basis_output_folder"].GetString()
        directory = Path(rom_output_folder_name)
        directory.mkdir(parents=True, exist_ok=True)
        self.database_name = directory / "rom_database.db"  # Adjust the database file name as needed
        conn = sqlite3.connect(self.database_name)
        cursor = conn.cursor()


        # Updated table definitions including new tables
        table_definitions = {
            "FOM": '''CREATE TABLE IF NOT EXISTS FOM
                        (id INTEGER PRIMARY KEY, parameters TEXT, file_name TEXT)''',
            "ROM": '''CREATE TABLE IF NOT EXISTS ROM
                        (id INTEGER PRIMARY KEY, parameters TEXT, tol_sol REAL,  type_of_projection TEXT,  file_name TEXT)''',
            "HROM": '''CREATE TABLE IF NOT EXISTS HROM
                        (id INTEGER PRIMARY KEY, parameters TEXT, tol_sol REAL, tol_res REAL, type_of_projection TEXT, file_name TEXT)''',
            "HHROM": '''CREATE TABLE IF NOT EXISTS HHROM
                        (id INTEGER PRIMARY KEY, parameters TEXT, tol_sol REAL, tol_res REAL, type_of_projection TEXT, file_name TEXT)''',
            "RightBasis": '''CREATE TABLE IF NOT EXISTS RightBasis
                        (id INTEGER PRIMARY KEY, tol_sol REAL, file_name TEXT)''',
            "LeftBasis": '''CREATE TABLE IF NOT EXISTS LeftBasis
                        (id INTEGER PRIMARY KEY, tol_sol REAL, basis_strategy TEXT, include_phi INTEGER, tol_pg REAL, solving_technique TEXT, monotonicity_preserving INTEGER , file_name TEXT)''',
            "PetrovGalerkinSnapshots": '''CREATE TABLE IF NOT EXISTS PetrovGalerkinSnapshots
                        (id INTEGER PRIMARY KEY, parameters TEXT, tol_sol REAL, basis_strategy TEXT, include_phi INTEGER, tol_pg REAL, solving_technique TEXT, monotonicity_preserving INTEGER , file_name TEXT)''',
            "ResidualsProjected": '''CREATE TABLE IF NOT EXISTS ResidualsProjected
                        (id INTEGER PRIMARY KEY, parameters TEXT, tol_sol REAL, tol_res REAL, type_of_projection TEXT, file_name TEXT)''',
            "HROM_Elements": '''CREATE TABLE IF NOT EXISTS HROM_Elements
                        (id INTEGER PRIMARY KEY, tol_sol REAL, tol_res REAL, type_of_projection TEXT, file_name TEXT)''',
            "HROM_Weights": '''CREATE TABLE IF NOT EXISTS HROM_Weights
                        (id INTEGER PRIMARY KEY, tol_sol REAL, tol_res REAL, type_of_projection TEXT, file_name TEXT)'''
        }


        self.table_names = table_definitions.keys()

        # Create each table using its definition
        for table_name, table_sql in table_definitions.items():
            try:
                cursor.execute(table_sql)
                conn.commit()
                print(f"Table {table_name} created successfully.")
            except sqlite3.OperationalError as e:
                print(f"Error creating table {table_name}: {e}")

        # Close the database connection
        conn.close()


    def hash_parameters(self, mu):
        # Convert parameters list to a string and encode
        mu_str = '_'.join(map(str, mu))
        return hashlib.sha256(mu_str.encode()).hexdigest()



    def hash_parameters_with_tol_and_projection(self, mu, tol, projection):
        # Convert the parameters list and the tolerance value to a string
        # Ensure the tolerance is converted to a string with consistent formatting
        mu_str = '_'.join(map(str, mu)) + f"_{tol:.10f}_" + projection   # Example with 10 decimal places
        # Generate the hash
        return hashlib.sha256(mu_str.encode()).hexdigest()


    def hash_parameters_with_tol_2_tols(self, mu, tol, tol2, projection):
        # Convert the parameters list to a string and encode
        mu_str = '_'.join(map(str, mu)) + f"_{tol:.10f}_{tol2:.10f}_" + projection  # Convert both tol and tol2 to strings with consistent formatting
        # Generate the hash of the combined string including both tolerance values
        return hashlib.sha256(mu_str.encode()).hexdigest()

    def hash_parameters_with_tol_2_tols_and_string(self, mu, tol, tol2, projection):
        # Convert the parameters list to a string and encode
        mu_str = '_'.join(map(str, mu)) + f"_{tol:.10f}_{tol2:.10f}_" +  projection # Convert both tol and tol2 to strings with consistent formatting
        # Generate the hash of the combined string including both tolerance values
        return hashlib.sha256(mu_str.encode()).hexdigest()


    def hash_mu_train(self, serialized_mu_train, real_value):
        """
        Generates a hash for the serialized mu_train, including a real value in the hash.
        """
        # Concatenate the serialized mu_train with the real_value (converted to string) for hashing
        # Ensure real_value is converted to a string with consistent formatting
        combined_str = f"{serialized_mu_train}_{real_value:.10f}"  # Example with 10 decimal places
        # Generate the hash of the combined string
        return hashlib.sha256(combined_str.encode()).hexdigest()


    def hash_mu_train_with_3_parms(self, serialized_mu_train, tol_sol,tol_res,projection_type):
        """
        Generates a hash for the serialized mu_train, including a real value in the hash.
        """
        # Concatenate the serialized mu_train with the real_value (converted to string) for hashing
        # Ensure real_value is converted to a string with consistent formatting
        combined_str_z = f"{serialized_mu_train}_{tol_sol:.10f}_{tol_res:.10f}_{projection_type}_z"  # Example with 10 decimal places
        combined_str_w = f"{serialized_mu_train}_{tol_sol:.10f}_{tol_res:.10f}_{projection_type}_w"  # Example with 10 decimal places
        # Generate the hash of the combined string
        return hashlib.sha256(combined_str_z.encode()).hexdigest(), hashlib.sha256(combined_str_w.encode()).hexdigest()


    def hash_parameters_with_6_params(self, mu, tol_sol, pg_data1_str,pg_data2_bool,pg_data3_double,pg_data4_str,pg_data5_bool):

        combined_str = '_'.join(map(str, mu)) + f"_{tol_sol:.10f}_" + pg_data1_str + "_" + str(pg_data2_bool) + f"_{pg_data3_double:.10f}_" + pg_data4_str + "_" + str(pg_data5_bool)
        # Generate the hash of the combined string
        return hashlib.sha256(combined_str.encode()).hexdigest()

    def hash_mu_train_with_6_params(self, serialized_mu_train, tol_sol, pg_data1_str,pg_data2_bool,pg_data3_double,pg_data4_str,pg_data5_bool):

        combined_str = serialized_mu_train + f"_{tol_sol:.10f}_" + pg_data1_str + "_" + str(pg_data2_bool) + f"_{pg_data3_double:.10f}_" + pg_data4_str + "_" + str(pg_data5_bool)

        return hashlib.sha256(combined_str.encode()).hexdigest()



    def check_if_mu_already_in_database(self, mu):
        conn = sqlite3.connect(self.database_name)
        cursor = conn.cursor()
        hash_mu = self.hash_parameters(mu)
        cursor.execute('SELECT COUNT(*) FROM FOM WHERE file_name = ?', (hash_mu,))
        count = cursor.fetchone()[0]
        conn.close()
        return count > 0, hash_mu


    def check_if_rom_already_in_database(self, mu, tol_sol):
        conn = sqlite3.connect(self.database_name)
        cursor = conn.cursor()
        projection_type = self.general_rom_manager_parameters["projection_strategy"].GetString()
        hash_mu = self.hash_parameters_with_tol_and_projection(mu, tol_sol, projection_type)
        cursor.execute('SELECT COUNT(*) FROM ROM WHERE file_name = ?', (hash_mu,))
        count = cursor.fetchone()[0]
        conn.close()
        return count > 0, hash_mu


    def check_if_hrom_already_in_database(self, mu, tol_sol, tol_res):
        conn = sqlite3.connect(self.database_name)
        cursor = conn.cursor()
        projection_type = self.general_rom_manager_parameters["projection_strategy"].GetString()
        hash_mu = self.hash_parameters_with_tol_2_tols(mu, tol_sol, tol_res, projection_type)
        cursor.execute('SELECT COUNT(*) FROM HROM WHERE file_name = ?', (hash_mu,))
        count = cursor.fetchone()[0]
        conn.close()
        return count > 0, hash_mu


    def check_if_right_basis_already_in_database(self, mu_train, tol_sol):
        conn = sqlite3.connect(self.database_name)
        cursor = conn.cursor()
        serialized_mu_train = self.serialize_entire_mu_train(mu_train)
        hashed_mu_train = self.hash_mu_train(serialized_mu_train, tol_sol)
        # Include both file_name and tol_sol in the WHERE clause
        cursor.execute('SELECT COUNT(*) FROM RightBasis WHERE file_name = ? AND tol_sol = ?', (hashed_mu_train, tol_sol))
        count = cursor.fetchone()[0]
        conn.close()
        return count > 0 , hashed_mu_train


    def check_if_left_basis_already_in_database(self, mu_train, tol_sol,pg_data1_str,pg_data2_bool,pg_data3_double,pg_data4_str,pg_data5_bool):
        conn = sqlite3.connect(self.database_name)
        cursor = conn.cursor()
        serialized_mu_train = self.serialize_entire_mu_train(mu_train)
        hashed_mu_train = self.hash_mu_train_with_6_params(serialized_mu_train, tol_sol, pg_data1_str,pg_data2_bool,pg_data3_double,pg_data4_str,pg_data5_bool)
        # Include both file_name and tol_sol in the WHERE clause
        cursor.execute('SELECT COUNT(*) FROM LeftBasis WHERE file_name = ? AND tol_sol = ?', (hashed_mu_train, tol_sol))
        count = cursor.fetchone()[0]
        conn.close()
        return count > 0 , hashed_mu_train


    def check_if_petrov_galerkin_already_in_database(self, mu, tol_sol,pg_data1_str,pg_data2_bool,pg_data3_double,pg_data4_str,pg_data5_bool):
        conn = sqlite3.connect(self.database_name)
        cursor = conn.cursor()
        projection_type = self.general_rom_manager_parameters["projection_strategy"].GetString()
        hash_mu = self.hash_parameters_with_6_params(mu, tol_sol, pg_data1_str,pg_data2_bool,pg_data3_double,pg_data4_str,pg_data5_bool)
        cursor.execute('SELECT COUNT(*) FROM PetrovGalerkinSnapshots WHERE file_name = ?', (hash_mu,))
        count = cursor.fetchone()[0]
        conn.close()
        return count > 0, hash_mu




    def check_if_res_already_in_database(self, mu, tol_sol, tol_res, projection_type):
        conn = sqlite3.connect(self.database_name)
        cursor = conn.cursor()
        hash_mu = self.hash_parameters_with_tol_2_tols_and_string(mu, tol_sol, tol_res, projection_type)
        cursor.execute('SELECT COUNT(*) FROM ResidualsProjected WHERE file_name = ?', (hash_mu,))
        count = cursor.fetchone()[0]
        conn.close()
        return count > 0, hash_mu


    def check_if_hrom_elems_and_weights_already_in_database(self, mu_train, tol_sol, tol_res, projection_type):
        conn = sqlite3.connect(self.database_name)
        cursor = conn.cursor()
        serialized_mu_train = self.serialize_entire_mu_train(mu_train)
        hashed_z, hashed_w = self.hash_mu_train_with_3_parms(serialized_mu_train, tol_sol,tol_res,projection_type)
        # Include both file_name and tol_sol in the WHERE clause
        cursor.execute('SELECT COUNT(*) FROM HROM_Elements WHERE file_name = ?', (hashed_z,))
        count = cursor.fetchone()[0]
        conn.close()
        return count > 0, hashed_z, hashed_w




    def add_FOM_to_database(self, parameters, file_name):
        conn = sqlite3.connect(self.database_name)
        cursor = conn.cursor()
        parameters_str = self.serialize_mu(parameters)
        cursor.execute('INSERT INTO FOM (parameters, file_name) VALUES (?, ?)',
                       (parameters_str, file_name))
        conn.commit()
        conn.close()

    def add_ROM_to_database(self, parameters, file_name, tol_sol, projection):
        conn = sqlite3.connect(self.database_name)
        cursor = conn.cursor()
        parameters_str = self.serialize_mu(parameters)
        cursor.execute('INSERT INTO ROM (parameters, tol_sol , type_of_projection, file_name) VALUES (?, ?, ?, ?)',
                       (parameters_str, tol_sol, projection, file_name))
        conn.commit()
        conn.close()


    def add_HROM_to_database(self, parameters, file_name, tol_sol, tol_res, projection):
        conn = sqlite3.connect(self.database_name)
        cursor = conn.cursor()
        parameters_str = self.serialize_mu(parameters)
        cursor.execute('INSERT INTO HROM (parameters, tol_sol , tol_res , type_of_projection, file_name) VALUES (?, ?, ?, ?, ?)',
                       (parameters_str, tol_sol, tol_res, projection, file_name))
        conn.commit()
        conn.close()


    def add_RightBasis_to_database(self, u,  mu_train, real_value):
        print(f"Attempting to add tol_sol with value: {real_value}")  # Debugging line

        serialized_mu_train = self.serialize_entire_mu_train(mu_train)
        hashed_mu_train = self.hash_mu_train(serialized_mu_train, real_value)
        file_path = self.save_as_npy(u , hashed_mu_train)  # Save numpy array and get file path
        conn = sqlite3.connect(self.database_name)
        cursor = conn.cursor()

        # Debugging: Print the values before executing the SQL command
        print(f"Inserting into RightBasis: tol_sol={real_value}, file_name={hashed_mu_train}")

        cursor.execute('INSERT INTO RightBasis (tol_sol, file_name) VALUES (?, ?)',
                    (real_value, str(hashed_mu_train)))
        conn.commit()
        conn.close()



    def add_LeftBasis_to_database(self, u, mu_train,tol_sol, pg_data1_str,pg_data2_bool,pg_data3_double,pg_data4_str,pg_data5_bool):

        serialized_mu_train = self.serialize_entire_mu_train(mu_train)
        hashed_mu_train = self.hash_mu_train_with_6_params(serialized_mu_train, tol_sol, pg_data1_str,pg_data2_bool,pg_data3_double,pg_data4_str,pg_data5_bool)

        file_path = self.save_as_npy(u , hashed_mu_train)  # Save numpy array and get file path
        conn = sqlite3.connect(self.database_name)
        cursor = conn.cursor()

        cursor.execute('INSERT INTO LeftBasis (tol_sol, basis_strategy, include_phi, tol_pg, solving_technique, monotonicity_preserving, file_name) VALUES (?, ?, ?, ?, ?, ?, ?)',
                    (tol_sol, pg_data1_str,pg_data2_bool, pg_data3_double, pg_data4_str,  pg_data5_bool, hashed_mu_train))
        conn.commit()
        conn.close()


    def add_petrov_galerkin_to_database(self, mu,file_name, tol_sol,pg_data1_str,pg_data2_bool,pg_data3_double,pg_data4_str,pg_data5_bool):
        conn = sqlite3.connect(self.database_name)
        cursor = conn.cursor()
        parameters_str = self.serialize_mu(mu)
        cursor.execute('INSERT INTO PetrovGalerkinSnapshots (parameters, tol_sol , basis_strategy, include_phi, tol_pg, solving_technique, monotonicity_preserving, file_name) VALUES (?, ?, ?, ?, ?, ?, ?, ?)',
                       (parameters_str, tol_sol, pg_data1_str,pg_data2_bool, pg_data3_double, pg_data4_str,  pg_data5_bool, file_name))
        conn.commit()
        conn.close()




    def add_ResidualProjected_to_database(self, parameters, file_name, tol_sol, tol_res, type_of_projection):
        conn = sqlite3.connect(self.database_name)
        cursor = conn.cursor()
        parameters_str = self.serialize_mu(parameters)
        cursor.execute('INSERT INTO ResidualsProjected (parameters, type_of_projection, tol_sol , tol_res , file_name) VALUES (?, ?, ?, ?, ?)',
                       (parameters_str, type_of_projection, tol_sol, tol_res, file_name))
        conn.commit()
        conn.close()


    def add_elements_and_weights_to_database(self, tol_sol,tol_res, projection_type,z,w):
        conn = sqlite3.connect(self.database_name)
        cursor = conn.cursor()
        cursor.execute('INSERT INTO HROM_Elements (file_name, tol_sol , tol_res , type_of_projection) VALUES (?, ?, ?, ?)', (z, tol_sol, tol_res, projection_type))
        cursor.execute('INSERT INTO HROM_Weights  (file_name, tol_sol , tol_res , type_of_projection) VALUES (?, ?, ?, ?)', (w, tol_sol, tol_res, projection_type))
        conn.commit()
        conn.close()


    def serialize_mu(self, parameters):
        return json.dumps(parameters)


    def save_as_npy(self, result, hash_mu):
        rom_output_folder_name = self.rom_training_parameters["Parameters"]["rom_basis_output_folder"].GetString()
        directory = Path(rom_output_folder_name) / 'rom_database'  #TODO hardcoded names here
        file_path = directory / f"{hash_mu}.npy"
        directory.mkdir(parents=True, exist_ok=True)
        np.save(file_path, result)
        return file_path


    def FOM_make_mu_dictionary(self, mu):
        if self.mu_names is None:
            self.mu_names = []
            for i in range(len(mu)):
                self.mu_names.append(f'generic_name_{i}')
        return dict(zip(self.mu_names , mu))

    def RightBasis_make_dictionary(self, mu):
        keys = ["hashed_name", "svd_tol_solution"]
        return dict(zip(keys, mu))


    def get_snapshots_matrix_from_database(self, mu_list, table_name='FOM'):
        unique_tuples = set(tuple(item) for item in mu_list)
        mu_list_unique = [list(item) for item in unique_tuples] #unique members in mu_lust
        rom_output_folder_name = self.rom_training_parameters["Parameters"]["rom_basis_output_folder"].GetString()
        directory = Path(rom_output_folder_name) / 'rom_database' #TODO hardcoded names here
        directory.mkdir(parents=True, exist_ok=True)
        SnapshotsMatrix = []
        conn = sqlite3.connect(self.database_name)
        cursor = conn.cursor()
        tol_sol = self.rom_training_parameters["Parameters"]["svd_truncation_tolerance"].GetDouble()
        tol_res =  self.hrom_training_parameters["element_selection_svd_truncation_tolerance"].GetDouble()
        projection_type = self.general_rom_manager_parameters["projection_strategy"].GetString()
        pg_data1_str = self.general_rom_manager_parameters["ROM"]["lspg_rom_bns_settings"]["basis_strategy"].GetString()
        pg_data2_bool = self.general_rom_manager_parameters["ROM"]["lspg_rom_bns_settings"]["include_phi"].GetBool()
        pg_data3_double = self.general_rom_manager_parameters["ROM"]["lspg_rom_bns_settings"]["svd_truncation_tolerance"].GetDouble()
        pg_data4_str = self.general_rom_manager_parameters["ROM"]["lspg_rom_bns_settings"]["solving_technique"].GetString()
        pg_data5_bool = self.general_rom_manager_parameters["ROM"]["lspg_rom_bns_settings"]["monotonicity_preserving"].GetBool()
        for mu in mu_list_unique:
            serialized_mu = self.serialize_mu(self.FOM_make_mu_dictionary(mu))
            if table_name == 'FOM':
                hash_mu = self.hash_parameters(serialized_mu)
            elif table_name == 'ROM':
                hash_mu = self.hash_parameters_with_tol_and_projection(serialized_mu, tol_sol, projection_type)
            elif table_name == 'HROM':
                hash_mu = self.hash_parameters_with_tol_2_tols(serialized_mu, tol_sol, tol_res, projection_type)
            elif table_name == 'ResidualsProjected':
                hash_mu = self.hash_parameters_with_tol_2_tols_and_string(serialized_mu, tol_sol, tol_res, projection_type)
            elif table_name == 'PetrovGalerkinSnapshots':
                hash_mu = self.hash_parameters_with_6_params(serialized_mu, tol_sol, pg_data1_str,pg_data2_bool,pg_data3_double,pg_data4_str,pg_data5_bool)
            cursor.execute(f"SELECT file_name FROM {table_name} WHERE file_name = ?", (hash_mu,)) # Query the database for the file name using the hash
            result = cursor.fetchone()

            if result:
                file_name = result[0]
                file_path = directory / (file_name + '.npy')
                SnapshotsMatrix.append(np.load(file_path))
            else:
                print(f"No entry found for hash {hash_mu}")

        conn.close()

        return np.block(SnapshotsMatrix) if SnapshotsMatrix else None


    def get_elements_and_weights(self, hash_w , hash_z):

        rom_output_folder_name = self.rom_training_parameters["Parameters"]["rom_basis_output_folder"].GetString()
        directory = Path(rom_output_folder_name) / 'rom_database' #TODO hardcoded names here
        directory.mkdir(parents=True, exist_ok=True)
        file_path = directory / (hash_z + '.npy')
        z = np.load(file_path)
        file_path = directory / (hash_w + '.npy')
        w = np.load(file_path)

        return w, z



    def serialize_entire_mu_train(self, mu_train):
        """
        Serializes the entire mu_train list of lists into a string.
        """
        # Ensure consistent serialization by sorting sublists if needed
        # Here, we assume mu_train's structure is consistent without needing sorting
        serialized_mu_train = json.dumps(mu_train, sort_keys=True)
        return serialized_mu_train


    def make_sure_basis_is_right(self, hash_basis):

        rom_output_folder_name = self.rom_training_parameters["Parameters"]["rom_basis_output_folder"].GetString()

        data_base_directory = Path(rom_output_folder_name) / 'rom_database' #TODO hardcoded names here
        data_base_directory.mkdir(parents=True, exist_ok=True)
        rom_directoy = Path(rom_output_folder_name)
        rom_directoy.mkdir(parents=True, exist_ok=True)
        currently_available_basis = np.load(rom_directoy / ('LeftBasisMatrix.npy'))
        basis_in_database = np.load(data_base_directory / (hash_basis + '.npy'))
        are_identical = np.array_equal(currently_available_basis, basis_in_database)
        if not are_identical:
            np.save(rom_directoy / ('LeftBasisMatrix.npy'), basis_in_database)


    def get_left_basis(self, hash_basis):

        rom_output_folder_name = self.rom_training_parameters["Parameters"]["rom_basis_output_folder"].GetString()
        data_base_directory = Path(rom_output_folder_name) / 'rom_database' #TODO hardcoded names here
        data_base_directory.mkdir(parents=True, exist_ok=True)

        return np.load(data_base_directory / (hash_basis + '.npy'))


    def get_right_basis(self, hash_basis):

        rom_output_folder_name = self.rom_training_parameters["Parameters"]["rom_basis_output_folder"].GetString()
        data_base_directory = Path(rom_output_folder_name) / 'rom_database' #TODO hardcoded names here
        data_base_directory.mkdir(parents=True, exist_ok=True)

        return np.load(data_base_directory / (hash_basis + '.npy'))


    def generate_excel(self, full_tables = False, number_of_terms=5):
        if missing_pandas == True:
            KratosMultiphysics.Logger.PrintWarning('\x1b[1;31m[MISSING LIBRARY] \x1b[0m'," pandas library not installed. No excel file generated")
        if missing_xlsxwriter == True:
            KratosMultiphysics.Logger.PrintWarning('\x1b[1;31m[MISSING LIBRARY] \x1b[0m'," xlsxwriter library not installed. No excel file generated")
        if missing_pandas==False and missing_xlsxwriter==False:
            conn = sqlite3.connect(self.database_name)
            # Query the database for table names
            query = "SELECT name FROM sqlite_master WHERE type='table';"
            tables = pd.read_sql_query(query, conn)

            if full_tables==False:
                output_folder = './Summary.xlsx'
            elif full_tables==True:
                output_folder = './DataBaseCompleteDump.xlsx'
            output_file_path = Path(output_folder)

            with pd.ExcelWriter(output_file_path, engine='xlsxwriter') as writer:
                for table_name in tables['name']:
                    if full_tables==True:
                        df = pd.read_sql_query(f"SELECT * FROM {table_name}", conn)
                    else:
                        df = pd.read_sql_query(f"SELECT * FROM {table_name}  LIMIT {number_of_terms}", conn)
                    df.to_excel(writer, sheet_name=table_name, index=False)
                    print(f"Exported {table_name} to Excel sheet.")
            conn.close()
            print(f"Data exported to Excel file {output_file_path} successfully.")

    def generate_database_summary(self):
        number_of_terms = 5 #TODO user-defined
        self.generate_excel(full_tables=False, number_of_terms=number_of_terms)

    def dump_database_as_excel(self):
        self.generate_excel(full_tables=True)