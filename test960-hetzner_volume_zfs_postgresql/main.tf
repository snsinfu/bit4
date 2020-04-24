resource "hcloud_server" "main" {
  name        = var.server_hostname
  location    = var.server_location
  server_type = var.server_type
  image       = var.server_image
  user_data   = data.template_file.startup_script.rendered
}

data "template_file" "startup_script" {
  template = file("${path.module}/startup.sh.in")
  vars = {
    admin_user            = var.admin_user
    admin_authorized_keys = join("\n", var.admin_public_keys)
    volume_id             = hcloud_volume.main.id
    benchmark_script      = file("${path.module}/benchmark.sh")
  }
}

resource "hcloud_volume" "main" {
  name     = var.volume_name
  size     = var.volume_size
  location = var.server_location
}

resource "hcloud_volume_attachment" "main" {
  volume_id = hcloud_volume.main.id
  server_id = hcloud_server.main.id
}
