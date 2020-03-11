resource "hcloud_server" "main" {
  name        = "main"
  location    = var.server_location
  server_type = var.server_type
  image       = "debian-10"
  user_data   = data.template_file.cloudinit.rendered
}

resource "hcloud_volume" "main" {
  name      = "main"
  size      = var.volume_size
  server_id = hcloud_server.main.id
}

data "template_file" "cloudinit" {
  template = file("${path.module}/cloudinit.yml.in")
  vars     = {
    admin_user         = var.admin_user
    admin_pubkeys_json = jsonencode(var.admin_pubkeys)
  }
}
