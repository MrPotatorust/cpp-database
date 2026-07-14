### Engine.cpp

- handles the queries, how they are routed and executed
- it also validates the input (if cols or rows match)

### Parser.cpp

- receives a query and tokenizes and parses it

### Database.cpp

- handles the data persistence
- acts as the main data store, storing all of the tables and data

### DBTypes.cpp

- stores some general data types related to database types, not parser types

### Helpers.cpp

- is designated for general helpers
- currently mostly stores converters for type enums or type resolving
