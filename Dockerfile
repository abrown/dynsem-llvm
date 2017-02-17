FROM ubuntu

# install common tools
RUN apt update
RUN apt install -y wget curl vim 

# setup clang repositories
RUN echo "deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial main" >> /etc/apt/sources.list.d/clang.list
RUN echo "deb-src http://apt.llvm.org/xenial/ llvm-toolchain-xenial main" >> /etc/apt/sources.list.d/clang.list
RUN wget -O - http://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -

# install clang
RUN apt update
RUN apt install -y clang-5.0 lldb-5.0
