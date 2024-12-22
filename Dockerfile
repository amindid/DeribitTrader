# Use an official Ubuntu image as the base
FROM ubuntu:20.04

# Set environment variables to suppress prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update package list and install essential build tools and dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    libboost-all-dev \
    libasio-dev \
    libcurl4-openssl-dev \
    wget \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory for the project
WORKDIR /app

# Clone nlohmann/json.hpp and WebSocket++ into the working directory
RUN git clone --depth 1 https://github.com/nlohmann/json.git /app/nlohmann && \
    git clone --depth 1 --branch master https://github.com/zaphoyd/websocketpp.git /app/websocketpp

# Copy the project files into the container
COPY ./src /app
COPY .env /app/.env

# Expose any necessary ports (e.g., for a WebSocket server)
EXPOSE 8080

# Set the default command to run the compiled executable
CMD ["make", "run"]
