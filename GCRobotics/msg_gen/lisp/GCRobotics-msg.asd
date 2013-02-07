
(cl:in-package :asdf)

(defsystem "GCRobotics-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "i2cData" :depends-on ("_package_i2cData"))
    (:file "_package_i2cData" :depends-on ("_package"))
  ))