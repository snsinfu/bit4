# FreeBSD/Linux difference on sticky-bit directory

When you (user u, group g) create a file in a shared directory with sticky bit
set (mode 1777), the owner (user:group) of the file will be:

- FreeBSD: `u:G`
- Linux: `u:g`

where G is the owner group of the shared directory.
