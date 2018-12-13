provider "aws" {
  region = "${var.cloud_region}"
}

#
# VPC
#

resource "aws_vpc" "default" {
  cidr_block = "10.0.0.0/16"
}

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
# Security group
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
# Autoscaling group
#

resource "aws_autoscaling_group" "main" {
  desired_capacity    = 1
  max_size            = 1
  min_size            = 1
  vpc_zone_identifier = ["${aws_subnet.default.id}"]

  launch_template = {
    id = "${aws_launch_template.main.id}"
  }
}

resource "aws_launch_template" "main" {
  name = "main"

  instance_type = "${var.instance_type}"
  image_id      = "${data.aws_ami.main.image_id}"
  user_data     = "${base64encode(data.template_file.cloudinit.rendered)}"

  network_interfaces {
    associate_public_ip_address = true
    security_groups             = ["${aws_security_group.ssh.id}"]
    subnet_id                   = "${aws_subnet.default.id}"
  }

  instance_market_options {
    market_type = "spot"
    spot_options {
      max_price = "${var.max_price}"
    }
  }
}

data "aws_ami" "main" {
  most_recent = true
  owners      = ["${var.image_owner}"]

  filter {
    name   = "name"
    values = ["${var.image_name}"]
  }
}

data "template_file" "cloudinit" {
  template = "${file("cloudinit.yml.in")}"

  vars {
    ssh_user    = "${var.ssh_user}"
    ssh_pubkeys = "${jsonencode(var.ssh_pubkeys)}"
  }
}
