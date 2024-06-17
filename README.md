# C Code Security Analysis

This project provides a security analysis report for your C code, identifying potential vulnerabilities and suggesting safer alternatives.

## Getting Started

Follow these instructions to set up and run the project.

### Prerequisites

Ensure you have Docker installed on your system. Docker allows you to create, deploy, and run applications using containers.
Docker is used in this project to ensure the correct environment before running the actual project.

### Setting Up the Project

1. **Build the Docker Image**

   Open a terminal and navigate to the directory containing your `Dockerfile`. Run the following command to build the Docker image:

   ```sh
   docker build --tag host --file Dockerfile .
   ```
   This command creates a Docker image with the tag host based on the instructions in the Dockerfile.

2. **Run the Docker Container**

	After the Docker image is built, run the following command to start a container from the image:
	```sh
	docker run -d -t --name pspp host
	```
	This command runs the Docker container in detached mode with a terminal allocated, naming the container pspp.
	
### You are Now ready to run the project.

With the Docker container running, the environment is set up correctly. 
You can now proceed to run the actual project.
