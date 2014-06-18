SET(LIBRARY_OUTPUT_PATH ${OUTPUT_DIR}/${MODULE_NAME})

AUX_SOURCE_DIRECTORY(. SRC_LIST)
ADD_LIBRARY(${MODULE_NAME} STATIC ${SRC_LIST})

TARGET_LINK_LIBRARIES(${MODULE_NAME} boost_chrono-mgw48-mt-1_55 boost_thread-mgw48-mt-1_55 boost_system-mgw48-mt-1_55 boost_date_time-mgw48-mt-1_55 boost_filesystem-mgw48-mt-1_55 boost_log-mgw48-mt-1_55 boost_log_setup-mgw48-mt-1_55 boost_regex-mgw48-mt-1_55 mysql TcAdsDll)
