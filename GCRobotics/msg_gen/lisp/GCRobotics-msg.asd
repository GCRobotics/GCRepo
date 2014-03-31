
(cl:in-package :asdf)

(defsystem "GCRobotics-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "command_state" :depends-on ("_package_command_state"))
    (:file "_package_command_state" :depends-on ("_package"))
    (:file "i2cData" :depends-on ("_package_i2cData"))
    (:file "_package_i2cData" :depends-on ("_package"))
    (:file "Encoder_msg" :depends-on ("_package_Encoder_msg"))
    (:file "_package_Encoder_msg" :depends-on ("_package"))
  ))