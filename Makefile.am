ACLOCAL_AMFLAGS = -I m4 --install

SUBDIRS = src

EXTRA_DIST = bootstrap.sh AUTHORS TODO NEWS

ChangeLog: NEWS
	git pull; git2cl > ChangeLog
