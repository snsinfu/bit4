resource "hcloud_server" "main" {
  name        = var.server_hostname
  server_type = var.server_type
  location    = var.server_location
  image       = var.server_image
  user_data   = data.template_file.cloudinit.rendered
}

data "template_file" "cloudinit" {
  template = file("${path.module}/cloudinit.yml.in")
  vars     = {
    admin_user         = var.admin_user
    admin_pubkeys_json = jsonencode(var.admin_pubkeys)
  }
}
