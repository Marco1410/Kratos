// KRATOS  ___|  |                   |                   |
//       \___ \  __|  __| |   |  __| __| |   |  __| _` | |
//             | |   |    |   | (    |   |   | |   (   | |
//       _____/ \__|_|   \__,_|\___|\__|\__,_|_|  \__,_|_| MECHANICS
//
//  License:		 BSD License
//					 license: structural_mechanics_application/license.txt
//
//  Main authors:    Vicente Mataix
//
#include "contact_structural_mechanics_application_variables.h"

namespace Kratos
{
typedef array_1d<double,3> Vector3;

// CONDITIONS
/* Mortar method */
KRATOS_CREATE_VARIABLE( std::vector<contact_container>*, CONTACT_CONTAINERS ) // A vector of which contains the structure which defines the contact conditions
KRATOS_CREATE_VARIABLE( Element::Pointer , ELEMENT_POINTER )                  // A pointer to the element belonging to this condition
KRATOS_CREATE_VARIABLE( int , INTEGRATION_ORDER_CONTACT )                     // The integration order computed in the contact
KRATOS_CREATE_VARIABLE( Matrix, MORTAR_CONTACT_OPERATOR )                     // Mortar Contact Operator
KRATOS_CREATE_VARIABLE( double, ACTIVE_CHECK_FACTOR )                         // The factor employed to consider an active/inactive node
KRATOS_CREATE_VARIABLE( double, CONSTANT_ACT_INACT )                          // The constant that is considered for the check of active or inactive (when 0 it doesn't accept traction)
KRATOS_CREATE_VARIABLE( double, WEIGHTED_GAP )                                // The integrated gap employed in mortar formulation

}
