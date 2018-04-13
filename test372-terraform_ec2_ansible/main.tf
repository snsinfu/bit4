resource "aws_instance" "builder" {
  ami             = "ami-60a4b21c" // 16.04 LTS amd64 hvm:ebs-ssd
  instance_type   = "t2.nano"
  key_name        = "${aws_key_pair.user.id}"
  security_groups = ["${aws_security_group.ssh.name}"]
}

resource "aws_key_pair" "user" {
  public_key = "${file("id_rsa.pub")}"
}

resource "aws_security_group" "ssh" {
  ingress {
    description = "Allow SSH access from everywhere"
    from_port   = 22
    to_port     = 22
    protocol    = "tcp"
    cidr_blocks = ["0.0.0.0/0"]
  }
}
