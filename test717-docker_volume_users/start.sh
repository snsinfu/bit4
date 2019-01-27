#!/bin/sh

exec docker run -it --rm -v ~/multiuser_home:/home multiuser
