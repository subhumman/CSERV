import sys
import subprocess

DOCKER_IMAGE = "proda-server"
DOCKER_CONTAINER = "proda-server-instance"
DOCKERFILE = "dockerfile"
PORT = "8080"

def run(cmd, check=True, shell=False):
    print(f"Running: {' '.join(cmd) if isinstance(cmd, list) else cmd}")
    result = subprocess.run(cmd, check=check, shell=shell)
    return result

def deploy():
    print("Building Docker image...")
    run(["docker", "build", "-t", DOCKER_IMAGE, "-f", DOCKERFILE, "."])
    print("Stopping old container (if exists)...")
    run(f"docker rm -f {DOCKER_CONTAINER} || true", shell=True)
    print("Starting new container...")
    run([
        "docker", "run", "-d",
        "--name", DOCKER_CONTAINER,
        "-p", f"{PORT}:{PORT}",
        DOCKER_IMAGE
    ])
    print("Deployment complete.")

def monitor():
    print("Tailing logs from container...")
    run(["docker", "logs", "-f", DOCKER_CONTAINER])

def restart():
    print("Restarting container...")
    run(f"docker restart {DOCKER_CONTAINER}", shell=True)
    print("Container restarted.")

if __name__ == "__main__":
    if "--deploy" in sys.argv:
        deploy()
    elif "--monitor" in sys.argv:
        monitor()
    elif "--restart" in sys.argv:
        restart()
    else:
        print("Usage: python script.py --deploy | --monitor | --restart")