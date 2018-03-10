Name:    hello
Version: 2.10
Release: 1%{?dist}
Summary: Program that prints a friendly greeting

License: GPLv3
URL:     https://www.gnu.org/software/%{name}/
Source:  https://ftp.gnu.org/gnu/%{name}/%{name}-%{version}.tar.gz

# %license shim for older systems.
# https://pagure.io/packaging-committee/issue/411
%{!?_licensedir:%global license %doc}

%description
The GNU Hello program produces a familiar, friendly greeting. Yes, this is
another implementation of the classic program that prints "Hello, world!"
when you run it.

%prep
%autosetup

%build
%configure --disable-nls
make %{?_smp_mflags}

%install
%make_install

%files
%license COPYING
%doc ABOUT-NLS AUTHORS ChangeLog ChangeLog.O INSTALL NEWS README README-dev README-release THANKS TODO
%{_infodir}/dir
%{_infodir}/hello.info.*
%{_mandir}/man1/hello.1.*
%{_bindir}/hello

%changelog
* Fri Mar  9 2018 snsinfu 2.10-1
- Created a package.
