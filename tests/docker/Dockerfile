# Use Ubuntu as base image
FROM ubuntu:latest

# Update package list and install vim and gcc
RUN apt-get update && \
    apt-get install -y \
    vim \
    gcc \
    make \
    git \
    libconfig-dev \
    && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /root

# Default command
CMD ["/bin/bash"]
