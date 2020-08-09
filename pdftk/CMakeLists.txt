############################
###        pdftk         ###
###                      ###
############################

##
## Pdf toolkit
##
set(PDFTK "pdftk")


add_executable(${PDFTK} "")

target_include_directories( ${PDFTK}
 PRIVATE
   ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${PDFTK}
PRIVATE
   pdfwriter.h
   pdfwriter.cpp

   po_stream.h   
   po_stream.cpp

   embedded_icc.cpp
   embedded_icc.h
   
   pdftk.cpp
)

##
## compile definitions
##
target_compile_definitions(${PDFTK}
 PRIVATE 
 
  "$<$<PLATFORM_ID:Windows>:_SBCS>"
  "$<$<PLATFORM_ID:Windows>:WIN32>"
  "$<$<PLATFORM_ID:Windows>:WIN64>"
)

##
## Position independent code in Linux
##
target_compile_options(${PDFTK}
  PRIVATE 
  "$<$<PLATFORM_ID:Linux>:-fPIC>"
  "$<$<PLATFORM_ID:Linux>:-std=c++11>"
  "$<$<PLATFORM_ID:Windows>:/W3>"
)

target_link_libraries(${PDFTK}
 PRIVATE

)
