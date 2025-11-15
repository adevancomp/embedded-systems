# Add Docker's official GPG key:
sudo apt-get update
sudo apt-get install ca-certificates curl
sudo install -m 0755 -d /etc/apt/keyrings
sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
sudo chmod a+r /etc/apt/keyrings/docker.asc

# Add the repository to Apt sources:
echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu \
  $(. /etc/os-release && echo "${UBUNTU_CODENAME:-$VERSION_CODENAME}") stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
sudo apt-get update

# Install the Docker packages.
sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
# Start the docker service
sudo systemctl start docker
# Verify status docker
sudo systemctl status docker
# Add your user to docker group (execute docker commands without sudo)
sudo usermod -aG docker $USER
# Path to Docker configuration file
DOCKER_CONFIG="/etc/docker/daemon.json"
# Create the file with new IP range
# Docker was on the same IP range as these system 
sudo tee "$DOCKER_CONFIG" > /dev/null <<EOF
{
    "default-address-pools":
    [
        { "base": "10.10.0.0/16", "size": 24 }
    ]
}
EOF

sudo systemctl restart docker.service
