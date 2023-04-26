# Function that parse the message on mqtt topic and append 
# data into the appropriate file

library(tidyverse)
library(rjson)
library(here)

distribute_data <- function(json_input) {
  
  data_msg <- fromJSON(json_input)
  
  # parse variables
  # get date-time
  # create path and filename
  # create dataframe
  # append to file
  for (vars in names(data_msg)) {
    if (vars == "brokerTime") {
      tmp_time <- lubridate::ymd_hms(data_msg$brokerTime)
    } else if (vars == "topic") {
      tmp_path <- data_msg$topic
      tmp_day <- lubridate::as_date(tmp_time)
      tmp_file <- paste0(tmp_day, ".", gsub("/", ".", x = tmp_path))
    } else if (vars == "payload") {
      tmp_data_payload <- as.data.frame(data_msg$payload)
      tmp_data_payload <- tmp_data_payload %>%
        mutate(DATETIME = tmp_time, .before = 1)
      tmp_data_payload_json <- toJSON(tmp_data_payload)
    } else {}
  }
  
  # create data directory if doesn't exists
  tmp_full_path <- paste0(here("src", "tmp_data"), "/", tmp_path)
  dir.create(tmp_full_path, recursive = TRUE, showWarnings = FALSE)
  
  # create file if doesn't exists
  tmp_full_filename <- paste0(tmp_full_path, "/", tmp_file)
  if (!file.exists(tmp_full_filename)){
    file.create(tmp_full_filename)
  }
  
  # append data to file
  write_lines(x = tmp_data_payload_json, file = tmp_full_filename, append = TRUE)
  
}