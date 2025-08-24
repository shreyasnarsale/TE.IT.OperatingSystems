#!/bin/bash

# Default address book file name
address_book="AddressBook.txt"

# Function to create a new address book
create_book() {
  read -p "Enter file name for address book: " new_book
  [ -e "$new_book" ] && rm "$new_book"  # Remove if it already exists
  echo -e "EmpID\tName\tAddress\tMobile\tSalary" > "$new_book"  # Add header
  address_book="$new_book"
  echo "Created '$address_book'."
}

# Function to validate 10-digit mobile number
validate_mobile() {
  [[ "$1" =~ ^[0-9]{10}$ ]]
}

# Function to insert a new record
insert_record() {
  read -p "Emp ID: " empid
  read -p "Name: " name
  read -p "Address: " address
  read -p "Mobile: " mobile
  read -p "Salary: " salary

  # Check for empty input
  if [ -z "$empid" ] || [ -z "$name" ] || [ -z "$address" ] || [ -z "$mobile" ] || [ -z "$salary" ]; then
    echo "All fields are required!"
    return
  fi

  # Check for unique Emp ID
  if grep -q -P "^$empid\t" "$address_book"; then
    echo "A record with Emp ID $empid already exists!"
    return
  fi

  # Validate mobile number
  if ! validate_mobile "$mobile"; then
    echo "Invalid mobile number. Must be 10 digits."
    return
  fi

  # Append new record
  echo -e "$empid\t$name\t$address\t$mobile\t$salary" >> "$address_book"
  echo "Record added successfully."
}

# Function to view all records in table format
view_book() {
  echo "---------- Address Book Contents ----------"
  column -t -s $'\t' "$address_book"
  echo "-------------------------------------------"
}

# Function to delete a record by Emp ID
delete_record() {
  read -p "Enter Emp ID of record to delete: " empid
  tmpfile=$(mktemp)
  grep -v -P "^$empid\t" "$address_book" > "$tmpfile" && mv "$tmpfile" "$address_book"
  echo "If existed, record with Emp ID $empid was removed."
}

# Function to modify a record by Emp ID
modify_record() {
  read -p "Enter Emp ID of record to modify: " empid

  # Check if record exists
  if ! grep -q -P "^$empid\t" "$address_book"; then
    echo "No record found with Emp ID $empid."
    return
  fi

  # Remove old record
  tmpfile=$(mktemp)
  grep -v -P "^$empid\t" "$address_book" > "$tmpfile" && mv "$tmpfile" "$address_book"

  echo "Enter new details for this Emp ID: $empid"
  read -p "Name: " name
  read -p "Address: " address
  read -p "Mobile: " mobile
  read -p "Salary: " salary

  # Validate
  if [ -z "$name" ] || [ -z "$address" ] || [ -z "$mobile" ] || [ -z "$salary" ]; then
    echo "All fields are required!"
    return
  fi

  if ! validate_mobile "$mobile"; then
    echo "Invalid mobile number. Must be 10 digits."
    return
  fi

  # Add updated record
  echo -e "$empid\t$name\t$address\t$mobile\t$salary" >> "$address_book"
  echo "Record updated successfully."
}

# Main program loop
while :; do
  echo -e "\nAddress Book Menu:"
  echo "1) Create address book"
  echo "2) View address book"
  echo "3) Insert a record"
  echo "4) Delete a record"
  echo "5) Modify a record"
  echo "6) Exit"
  read -p "Choose an option [1-6]: " opt

  case "$opt" in
    1) create_book ;;
    2) [ -e "$address_book" ] || { echo "No address book found. Please create one first."; continue; }; view_book ;;
    3) [ -e "$address_book" ] || { echo "No address book found. Please create one first."; continue; }; insert_record ;;
    4) [ -e "$address_book" ] || { echo "No address book found. Please create one first."; continue; }; delete_record ;;
    5) [ -e "$address_book" ] || { echo "No address book found. Please create one first."; continue; }; modify_record ;;
    6) echo "Exiting. Goodbye!"; break ;;
    *) echo "Invalid option. Try again." ;;
  esac
done
