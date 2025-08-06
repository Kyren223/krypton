# Style Guidelines

## Naming conventions

- Function names - StructClassName
- Struct names - StructName
- Field names - fieldName
- Enum names - EnumName_VariantName
- Variable names - variableName
- Global variable names - global_variable_name

## Formatting

- Pointers are always touching the type, not the variable, and not neither (`char* var;`)
- Same-line multi declarations are not allowed (`int a, b;`)
- 2 space indents on new scope
- Use K&R style and not Allman style (put braces on the same line as if statements)

### Temporary

The following formatting rules are temporary
They are needed to make the metagen generate headers correctly

Struct, enum and union declarations should be of the form:

```c
struct Name
{
  // Fields
}
```

Note the space after `Name`

Section headers (comments) must be of the form:

```
/// --- Name --- ///
```

Do not add comments to the direct line above a function/struct,
this will mess up the metagen, instead put the comment as
the first line inside the function/struct, indented by 2 spaces.

Here's an example:

```c
fn void DoesNothing() {
  // This function does nothing
}

```
