# cppwal
This is a program used to generate a color scheme based on a picture.
Inspiration came from using pywal on a slower computer and wanted to add the ability to auto-generate color schemes.

## Getting Started

### To build individual components
```
make build
make install
```

### To compile and install
```
make
```

### To set wallpaper and create color scheme
```
cwal -i {File location of wallpaper}
```

### To clear cached color schemes
```
cwal -c
```

### To set terminal transparency
```
cwal -a {integer}
```

### Pre-requisites

```
imagemagick
g++
feh
```
