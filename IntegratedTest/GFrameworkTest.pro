TEMPLATE = subdirs

SUBDIRS += \
            Engine \
            ../GEditor \
            ./Editor



 Editor.depends = Engine
 Editor.depends = GEditor

#CONFIG += ordered
