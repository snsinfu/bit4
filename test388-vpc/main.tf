resource "aws_vpc" "default" {
  cidr_block = "10.0.0.0/16"
}

#
# Network
#

resource "aws_subnet" "default" {
  vpc_id                  = "${aws_vpc.default.id}"
  cidr_block              = "10.0.0.0/24"
  map_public_ip_on_launch = true
}

resource "aws_internet_gateway" "default" {
  vpc_id = "${aws_vpc.default.id}"
}

resource "aws_route_table" "default" {
  vpc_id = "${aws_vpc.default.id}"

  route {
    cidr_block = "0.0.0.0/0"
    gateway_id = "${aws_internet_gateway.default.id}"
  }
}

resource "aws_route_table_association" "default" {
  subnet_id      = "${aws_subnet.default.id}"
  route_table_id = "${aws_route_table.default.id}"
}

#
# Security
#

resource "aws_security_group" "ssh" {
  vpc_id = "${aws_vpc.default.id}"

  ingress {
    from_port   = 22
    to_port     = 22
    protocol    = "tcp"
    cidr_blocks = ["0.0.0.0/0"]
  }

  egress {
    from_port   = 0
    to_port     = 0
    protocol    = "-1"
    cidr_blocks = ["0.0.0.0/0"]
  }

  depends_on = ["aws_internet_gateway.default"]
}

#
# Instance
#

resource "aws_instance" "workstation" {
  instance_type          = "t2.micro"
  ami                    = "${data.aws_ami.ubuntu.image_id}"
  key_name               = "${aws_key_pair.auth.key_name}"
  vpc_security_group_ids = ["${aws_security_group.ssh.id}"]
  subnet_id              = "${aws_subnet.default.id}"
}

resource "aws_key_pair" "auth" {
  key_name   = "${aws_vpc.default.id}-userkey"
  public_key = "${file("id_rsa.pub")}"
}

data "aws_ami" "ubuntu" {
  most_recent = true
  owners      = ["099720109477"] # Canonical

  filter {
    name   = "name"
    values = ["ubuntu/images/hvm-ssd/ubuntu-xenial-16.04-amd64-server-*"]
  }

  filter {
    name   = "virtualization-type"
    values = ["hvm"]
  }
}

output "ip" {
  value = "${aws_instance.workstation.public_ip}"
}
