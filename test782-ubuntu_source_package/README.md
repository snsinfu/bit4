# Packaging GNU Hello on Ubuntu

Launch a Ubuntu VM on the host:

```
vagrant up
vagrant ssh
```

Then, run scripts in the VM:

```
cp -r /vagrant/hello .
cd hello
./aptget.sh
./fetch.sh
./build.sh
```

A binary package should be produced in the home directory in the VM.

## Packaging open source application

Fork the original repository and add `debian` directory containing four
mandatory files: `changelog`, `control`, `copyright` and `rules`. Then, run
`debuild -b -uc -us` command.

```
app
│
├── debian          -
│   ├── changelog    |
│   ├── control      | --- Package maintainer adds these files.
│   ├── copyright    |
│   └── rules       -
│
├── src             -
├── lib              |
├── Makefile.in      | --- Original source files.
├── configure        |
└── ...             -
```

`changelog` describes the update history (including upstream version) in a
strictly defined format. `control` contains metadata. `copyright` is a copy of
the upstream license. `rules` is a makefile for building the application.

## Resources

- https://www.debian.org/doc/debian-policy/index.html
- https://wiki.debian.org/BuildingTutorial
