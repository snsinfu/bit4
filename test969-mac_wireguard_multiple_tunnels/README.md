Wireguard GUI client for mac can manage only one tunnel at a time. This is not
a technical restriction. A wireguard tunnel is just a tun interface, and the
kernel supports multiple tun interfaces.

Still wireguard-go and the wrapper script wg-quick are capable of managing
multiple tunnels on mac. So, let's register wg-quick as a launchd service!

Install commands:

```
brew install wireguard-go wireguard-tools
cp wg-quick-as-service /usr/local/sbin/
```

Put interface config files in `/usr/local/etc/wireguard` directory.

Install service (copy service file and change `wg0` to other interfaces you
configured and copy all them into the same directory):

```
sudo cp wg-quick.wg0.plist /Library/LaunchDaemons/
sudo launchctl load /Library/LaunchDaemons/wg-quick.wg0.plist
```

Start and stop tunneling via each interface:

```
sudo launchctl start wg-quick.wg0
sudo launchctl stop wg-quick.wg0
```

Uninstall service:

```
sudo launchctl unload /Library/LaunchDaemons/wg-quick.wg0.plist
sudo rm /Library/LaunchDaemons/wg-quick.wg0.plist
```
