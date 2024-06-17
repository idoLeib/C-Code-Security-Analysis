FROM ubuntu:22.04
RUN apt-get update -y
RUN apt-get install git -y
RUN apt-get install vim -y
RUN apt-get install gcc -y
RUN apt-get install wget -y
RUN apt-get install meson -y
RUN apt-get install texinfo -y
RUN apt-get install gettext -y
RUN apt-get install libgsl-dev -y
RUN apt-get install zlib1g-dev -y
RUN apt-get install libxml2-dev -y
RUN apt-get install libglib2.0-dev -y
RUN git clone git://anongit.freedesktop.org/git/cairo
RUN cd cairo && meson setup build && ninja -C build && ninja -C build install
RUN wget https://download.gnome.org/sources/pango/1.50/pango-1.50.14.tar.xz
RUN tar -xf pango-1.50.14.tar.xz
RUN cd pango-1.50.14 && meson setup build && ninja -C build && ninja -C build install
RUN wget https://ftp.gnu.org/gnu/pspp/pspp-1.6.2.tar.gz
RUN tar -xf pspp-1.6.2.tar.gz
RUN cd pspp-1.6.2 && ./configure --without-gui CFLAGS="-g -O0"
RUN cd pspp-1.6.2 && make && make install
RUN objdump -D -S /usr/local/lib/pspp/libpspp-1.6.2.so > pspp.asm
