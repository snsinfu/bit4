resource "hcloud_server" "master" {
  name        = var.server_hostname
  server_type = var.server_type
  location    = var.server_location
  image       = var.server_image
  iso         = var.server_iso
  user_data   = data.template_file.imprint.rendered
}

data "template_file" "imprint" {
  template = file("${path.module}/assets/imprint.sh.in")
  vars     = {
    firstboot_base64 = base64encode(file("${path.module}/assets/firstboot.sh"))
  }
}
