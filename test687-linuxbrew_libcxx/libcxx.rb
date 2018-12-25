class Libcxx < Formula
  desc "LLVM libc++ and libc++abi"
  homepage "https://libcxx.llvm.org/"
  version "7.0.0"

  url "https://releases.llvm.org/7.0.0/llvm-7.0.0.src.tar.xz"
  sha256 "8bc1f844e6cbde1b652c19c1edebc1864456fd9c78b8c1bea038e51b363fe222"

  resource "libcxx" do
    url "https://releases.llvm.org/7.0.0/libcxx-7.0.0.src.tar.xz"
    sha256 "9b342625ba2f4e65b52764ab2061e116c0337db2179c6bce7f9a0d70c52134f0"
  end

  resource "libcxxabi" do
    url "https://releases.llvm.org/7.0.0/libcxxabi-7.0.0.src.tar.xz"
    sha256 "9b45c759ff397512eae4d938ff82827b1bd7ccba49920777e5b5e460baeb245f"
  end

  depends_on "cmake" => :build
  depends_on "llvm" => :build

  def install
    (buildpath/"projects/libcxx").install resource("libcxx")
    (buildpath/"projects/libcxxabi").install resource("libcxxabi")

    mkdir "build" do
      args = %W[
        -DCMAKE_C_COMPILER=clang
        -DCMAKE_CXX_COMPILER=clang++
      ]
      system "cmake", "-G", "Unix Makefiles", "..", *(std_cmake_args + args)
      system "make", "cxxabi"
      system "make", "cxx"
      system "make", "install-cxx", "install-cxxabi"
    end
  end
end
