resource "aws_lightsail_instance" "sail" {
  name              = "lightsail"
  availability_zone = var.server_zone
  blueprint_id      = var.server_blueprint
  bundle_id         = var.server_bundle
  user_data         = data.template_file.cloudinit.rendered
}

data "template_file" "cloudinit" {
  template = file("${path.module}/cloudinit.sh.in")
  vars     = {
    admin_user         = var.admin_user
    admin_password     = var.admin_password
    admin_pubkeys_text = join("\n", var.admin_pubkeys)
  }
}
