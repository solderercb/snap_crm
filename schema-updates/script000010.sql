INSERT INTO `permissions` (`id`, `name`) VALUES
  (101, 'editRepairIncomingSet'),
  (102, 'editAnyComment'),
  (103, 'advEditWorkList'),
  (104, 'viewClientPassportData'),
  (105, 'viewClientBankData'),
  (106, 'editPaymentSystemInCommittedCashRegisters'),
  (107, 'viewEmployeesSalary');

INSERT INTO `permissions_roles` (`permission_id`, `role_id`) VALUES
  (101, 1),
  (102, 1),
  (103, 1),
  (104, 1),
  (105, 1),
  (106, 1),
  (107, 1);