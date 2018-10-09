variable "image_id" {
}

resource "hcloud_ssh_key" "main" {
  name       = "main key"
  public_key = "${file("id_rsa.pub")}"
}

resource "hcloud_server" "main" {
  name        = "main"
  location    = "fsn1"
  server_type = "cx11"
  image       = "${var.image_id}"
  ssh_keys    = ["${hcloud_ssh_key.main.id}"]
}

output "ip" {
  value = "${hcloud_server.main.ipv4_address}"
}
