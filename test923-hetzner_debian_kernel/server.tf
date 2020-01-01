resource "hcloud_server" "master" {
  name        = local.hostname
  server_type = var.server_type
  location    = var.server_location
  image       = var.server_image
  user_data   = data.template_file.cloudinit.rendered
}

data "template_file" "cloudinit" {
  template = file("${path.module}/templates/cloudinit.yml.in")
  vars     = {
    admin_user         = var.admin_user
    admin_pubkeys_json = jsonencode(var.admin_pubkeys)
  }
}

resource "random_id" "host_id" {
  byte_length = 4
}

locals {
  hostname = "test-${random_id.host_id.hex}"
}

#
# Output
#

data "template_file" "ssh_config" {
  template = file("${path.module}/templates/ssh_config.in")
  vars     = {
    admin_user     = var.admin_user
    master_address = hcloud_server.master.ipv4_address
  }
}

output "ssh_config" {
  value     = data.template_file.ssh_config.rendered
  sensitive = true
}

data "template_file" "ansible_inventory" {
  template = file("${path.module}/templates/terraform_hosts.in")
  vars     = {
    admin_user     = var.admin_user
    master_address = hcloud_server.master.ipv4_address
  }
}

output "ansible_inventory" {
  value     = data.template_file.ansible_inventory.rendered
  sensitive = true
}
