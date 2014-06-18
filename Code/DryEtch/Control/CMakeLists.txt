SET(LIBRARY_OUTPUT_PATH ${OUTPUT_DIR}/${MODULE_NAME})

AUX_SOURCE_DIRECTORY(. SRC_LIST)
ADD_LIBRARY(${MODULE_NAME} SHARED ${SRC_LIST})

TARGET_LINK_LIBRARIES(${MODULE_NAME} Common boost_chrono-mgw48-mt-1_55 boost_thread-mgw48-mt-1_55 boost_system-mgw48-mt-1_55 boost_date_time-mgw48-mt-1_55 boost_filesystem-mgw48-mt-1_55 boost_log-mgw48-mt-1_55 boost_log_setup-mgw48-mt-1_55 boost_regex-mgw48-mt-1_55 mysql TcAdsDll)

EXECUTE_PROCESS(COMMAND python ${MAIN_DIR}/Script/parameters.py ${MAIN_DIR}/Code/${MODULE_NAME}/Control/Parameters.cpp ${OUTPUT_DIR}/${MODULE_NAME}/config/parameters.xml)
EXECUTE_PROCESS(COMMAND python ${MAIN_DIR}/Script/systemdata.py ${MAIN_DIR}/Code/${MODULE_NAME}/Control/Data.cpp ${OUTPUT_DIR}/${MODULE_NAME}/config/systemdata.xml)

#INSTALL(DIRECTORY ${OUTPUT_DIR}/Common/ DESTINATION ${OUTPUT_DIR}/${MODULE_NAME} FILES_MATCHING PATTERN "*.dll")
INSTALL(DIRECTORY ${MAIN_DIR}/Bin/ DESTINATION ${OUTPUT_DIR}/${MODULE_NAME})
