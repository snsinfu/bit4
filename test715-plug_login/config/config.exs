# This file is responsible for configuring your application
# and its dependencies with the aid of the Mix.Config module.
use Mix.Config

# This configuration is loaded before any dependency and is restricted
# to this project. If another project depends on this project, this
# file won't be loaded nor affect the parent project. For this reason,
# if you want to provide default values for your application for
# third-party users, it should be done in your "mix.exs" file.

# You can configure your application as:
#
#     config :login_app, key: :value
#
# and access this configuration in your application as:
#
#     Application.get_env(:login_app, :key)
#
# You can also configure a third-party app:
#
#     config :logger, level: :info
#

config :login_app, default_port: 4000
config :login_app, server_signature: "LoginApp"
config :login_app, session_encryption_salt: "jBEo0YbfPf2Q2SQnub+XdreU"
config :login_app, session_signing_salt: "O7giw/AV+txqHtFby7UkFeWR"

config :login_app,
  session_secret: "SsAeottSXieSUhG9j1XNNMUkuvqHClyurR2+zGfhYpb5MZEHuQA1JLgY9SAjzT06"

# It is also possible to import configuration files, relative to this
# directory. For example, you can emulate configuration per environment
# by uncommenting the line below and defining dev.exs, test.exs and such.
# Configuration from the imported file will override the ones defined
# here (which is why it is important to import them last).
#
#     import_config "#{Mix.env()}.exs"
