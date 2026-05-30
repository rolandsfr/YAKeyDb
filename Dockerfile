FROM gcc:14
RUN apt-get update && apt-get install -y gdb make && rm -rf /var/lib/apt/lists/*
WORKDIR /app
RUN printf 'source /app/.gdbinit\n' > /root/.gdbinit