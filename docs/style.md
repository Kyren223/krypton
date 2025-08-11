# Style Guidelines

## Naming conventions

- Function names - StructClassName
- Struct names - StructName
- Field names - fieldName
- Enum names - EnumName_variantName
- Variable names - variableName
- Global variable names - global_variable_name

## Formatting

- Pointers are always touching the type, not the variable, and not neither (`char* var;`)
- Same-line multi declarations are not allowed (`int a, b;`)
- 2 space indents on new scope
- Use K&R style and not Allman style (put braces on the same line as if statements)
- Section headers (comments) must be of the form `/// --- Name --- ///`
