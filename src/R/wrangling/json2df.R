# Import data from json file
# Created: 12 Apr 2023
# F. Dallo

library(tidyverse) # R tidy syntax
#library(dplyr) # dataframe manipulation
#library(readr) # read rectangular data
library(rjson) # interact with JavaScript Object Notation
library(here) # path relative to project

rm(list = ls())

# load function
source(here("src","R", "wrangling", "fun_distribute.R"))

# example with data
data_raw <- read_lines(file = here("src", "tmp_data", "tmp_db.json"))

for (line in seq(length(data_raw))) {
  distribute_data(data_raw[[line]])
}
