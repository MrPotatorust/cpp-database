This is just a quick overview for the functions and their "syntax rules", this implementation isnt ideal for longterm changes and implementation but I am trying to implement it myself.

TO DO: GROUP BY/ORDER BY
!!!!!!!!!!!!!!!!!!!!!!!!!! IMPORTANT, CURRENTLY EACH WORD OR TOKEN HAS TO BE SEPERATED BY AN EMPTY S


CONDITIONAL: WHERE condition (identifier)


FUNCTIONS

"CREATE"

Function identifier: "CREATE TABLE"

Example:
CREATE TABLE table_name (
col_name1 type,
col_name2 type,
col_name3 type,
);

"DROP TABLE"

Function identifier: "DROP TABLE"

Example:
DROP TABLE table_name;

"TRUNCATE TABLE"

Function identifier: "TRUNCATE TABLE"

Example:
TRUNCATE TABLE table_name;

"DELETE"

Function identifier: "DELETE FROM"
Potential: conditional WHERE

Example:
DELETE FROM table_name WHERE condition;

"UPDATE"

Function identifier: "UPDATE"
Potential: conditional WHERE

Example:
UPDATE table_name
SET column1 = value1, column2 = value2,
WHERE condition;

"SELECT"

Function identifier: "SELECT"
Potential: conditional WHERE

Example:
SELECT col_name1, col_name2 / \* FROM Customers WHERE condition;
