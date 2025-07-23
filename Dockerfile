# Use official Ubuntu image with clang and make
FROM ubuntu:22.04
# Install build tools
RUN apt-get update && \
    apt-get install -y clang make && \
    rm -rf /var/lib/apt/lists/*
# Set working directory
WORKDIR /app
# Copy all project files
COPY . .
# Build the project
RUN make all
# Expose the HTTP server port
EXPOSE 8080
# Run the server by default
CMD ["bin/proda"]