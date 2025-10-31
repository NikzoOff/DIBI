# Maintainer: Your Name <your.email@example.com>
pkgname=dibi
pkgver=1.0.0  # Your version
pkgrel=1
pkgdesc="Dibi or 'Did-I-Break-It' is a simple snapshot tool written in C. It will create a .DIBI file in your $HOME directory and a snapshot direcotry. It'll run basic tests and compare them."
arch=('x86_64')
url="https://github.com/yourusername/dibi"  # Your repo
license=('MIT')  # Or whatever license you use
depends=('glibc' 'systemd')  # Add your dependencies
makedepends=('git' 'gcc' 'make')  # Build tools
source=("$pkgname-$pkgver.tar.gz::$url/archive/v$pkgver.tar.gz")
sha256sums=('SKIP')  # Generate this later

build() {
    cd "$pkgname-$pkgver"
    make
}

package() {
    cd "$pkgname-$pkgver"
    make DESTDIR="$pkgdir/" install
}
