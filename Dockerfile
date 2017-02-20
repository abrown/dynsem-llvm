FROM ubuntu

# install common tools
RUN apt update
RUN apt install -y wget curl vim netcat

# setup clang repositories
RUN echo "deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial main" >> /etc/apt/sources.list.d/clang.list
RUN echo "deb-src http://apt.llvm.org/xenial/ llvm-toolchain-xenial main" >> /etc/apt/sources.list.d/clang.list
RUN wget -O - http://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -

# install clang
RUN apt update
RUN apt install -y clang-5.0 lldb-5.0 make

# open up SSH (insecurely); use http://superuser.com/a/433621 to disable host key checking to 10.0.75.2
RUN apt install -y openssh-server
EXPOSE 22
# change /etc/ssh/sshd_config to 'PermitRootLogin yes'
RUN sed -i 's/PermitRootLogin.*/PermitRootLogin yes/' /etc/ssh/sshd_config
# use 'root' as password
RUN echo 'root:root' | chpasswd

CMD /lib/systemd/systemd
