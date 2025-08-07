# StoreX

A simple C++ database implementation that supports storing text and binary data (such as PNG images) in a custom file format. Includes basic CRUD operations and file persistence.

## Features
- In-memory database with custom serialization
- Supports string and binary fields
- Can store and extract PNG files
- Uses a single `.storex` file for database storage
- Example usage in `main.cpp`

## Usage
1. **Add an image to the database:**
   - Place a PNG file named `sample.png` in the project folder.
   - Run the program to store the image in the database file.
2. **Extract images from the database:**
   - Remove or rename `sample.png`.
   - Run the program to extract images from the database file and save them to the current folder.

## Build
```
g++ -std=c++17 Database.cpp main.cpp -o db_app
```

## Run
```
./db_app
```

## License
MIT
