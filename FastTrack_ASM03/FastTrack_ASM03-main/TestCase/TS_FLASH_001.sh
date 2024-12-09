# Include source depend
source ./Makefile_checker.sh

# Go to path contain makefile
cd ../Project_Demo
clear

# ======================================
# Execute Function
# ======================================

# Check Build Project on Flash
Build_On "FLASH"

Check_Build_2nd "FLASH"

Check_Build_Modify "FLASH"
