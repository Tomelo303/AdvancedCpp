# CMakeList.txt : CMake project for ZaawansowanyCpp, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (ZaawansowanyCpp
 "ZaawansowanyCpp.cpp"
 
 "Lista1/Zadanie1_1.h"
 "Lista1/Zadanie1_2.h"
 "Lista1/Zadanie1_3.h"

 "Lista2/Zadanie2_1.h"
 "Lista2/Zadanie2_2.h"
 "Lista2/Zadanie2_3.h"

 "Lista3/Zadanie3_1.h"
 "Lista3/Zadanie3_2.h"
 "Lista3/Zadanie3_3.h"
 "Lista3/Forward_list_old.h"
 "Lista3/Forward_list_new_idx.h"
 "Lista3/Forward_list_new_key.h"
 
 "Lista4/Zadanie4_1.h"
 "Lista4/Zadanie4_2.h"

 "Lista5/Zadanie5_1.h"
 "Lista5/Zadanie5_2.h"
 "Lista5/Zadanie5_3.h"
 "Lista6/Zadanie6_1.h" "Lista6/Zadanie6_2.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ZaawansowanyCpp PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
