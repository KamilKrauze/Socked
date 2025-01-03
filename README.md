# Socked Library
> A portable C/C++ sockets library

> ### Supported Platforms
> 
> |Platforms     |Supports?      |Tested   |
> |--------------|---------------|---------|
> |Windows 10/11 |☑             |Yes       |
> |Linux         |☑ Partially   |Partially |
> |MacOS         |N/A            |N/A      |
>
> <br>

> ### Bindings
> Currently there is WIP [Odin](https://github.com/odin-lang/Odin) binding called [Socked-Odin](https://github.com/KamilKrauze/Socked-Odin.git) where, the library compiles down to a static library with [C functions that Odin links to](https://odin-lang.org/news/binding-to-c/) . However, you are free to use the [master-staticlib](https://www.github.com/KamilKrauze/Socked/tree/master-staticlib)

## Why use it?
Currently the C/C++ standard library doesn't have its own portable standard implementation for network socket programming.
While there are other more popular libraries that do achieve this, I figured that I should create my own to learn socket programming while also making it portable to the best of my ability