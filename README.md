# CSE381

This is a repository to hold the toy OS developed by Naoki, Patrick, and Bang



## Bootloader

    $ make
    $ make run

## PintOS
The Perl script does not work by default. The following fix is necessary: This fix only works for Threads. See [Wiki home](https://github.com/proctopj/CSE381/wiki#pintos-issues) for issues.

```diff
diff --git a/src/utils/Pintos.pm b/src/utils/Pintos.pm
index 70df40d..685aa37 100644
--- a/src/utils/Pintos.pm
+++ b/src/utils/Pintos.pm
@@ -47,7 +47,7 @@ sub set_part {
    my ($role, $source) = $opt =~ /^([a-z]+)(?:-([a-z]+))?/ or die;

    $role = uc $role;
-   $source = 'FILE' if $source eq '';
+   $source = 'FILE' if not defined $source or $source eq '';

    die "can't have two sources for \L$role\E partition"
    if exists $parts{$role};
@@ -64,7 +64,7 @@ sub do_set_part {
    my ($role, $source, $arg) = @_;

    my ($p) = $parts{$role} = {};
-   if ($source eq 'file') {
+   if ($source eq 'file' or $source eq 'FILE') {
        if (read_mbr ($arg)) {
            print STDERR "warning: $arg looks like a partitioned disk ";
            print STDERR "(did you want --$role-from=$arg or --disk=$arg?)\n"
```

Run pintos with:

    BUILD="$HOME/src/cse381/src/threads/build" pintos --kernel=$BUILD/kernel.bin --loader=$BUILD/loader.bin -v -k --qemu -- -q run alarm-multiple

but point to the appropriate `kernel.bin` and `loader.bin`.
