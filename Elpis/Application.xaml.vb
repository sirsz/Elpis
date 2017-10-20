Class Application

    ' Application-level events, such as Startup, Exit, and DispatcherUnhandledException
    ' can be handled in this file.
    Public config_ As New ConfigurationData()
    Dim dataReaderModule_ As New DataReaderModule()
    Public DatabaseSqliteInterface_ As New DbSqliteInterface
    Public authenticated_ As Boolean
    Public context_ As New Context

    Public dataExporter_ As DataExporter

    Public dataLoaderModule_ As New DataLoaderModule()

    Sub Application_Exit()
        Console.WriteLine("[Application] Has been closed. Bye")
    End Sub

    Sub LoadPersistentConfiguration()

        ' Data base settings:
        'config_.SettingsDataBaseIsInitialized_ = My.Settings.SettingsDataBaseIsInitialized
        config_.SettingsDataBasePath_ = My.Settings.SettingsDataBasePath

        config_.SettingsAutoImportEnabled_ = My.Settings.SettingsAutoImportEnabled
        config_.SettingsHospitalName_ = My.Settings.SettingsHospitalName
        config_.SettingsAutoImportTimePeriodInMins_ = My.Settings.SettingsAutoImportTimePeriodInMins
        config_.SettingsAutoImportFilePathWithDataForImporting_ = My.Settings.SettingsAutoImportFilePathWithDataForImporting



        config_.SettingsInitData_Ward_Loaded_ = My.Settings.Settings_InitData_Ward_Loaded
        config_.SettingsInitData_Ward_PathToFile_ = My.Settings.Settings_InitData_Ward_PathToFile

        config_.SettingsInitData_Room_Loaded_ = My.Settings.Settings_InitData_Room_Loaded
        config_.SettingsInitData_Room_PathToFile_ = My.Settings.Settings_InitData_Room_PathToFile

        config_.SettingsInitData_Staff_Loaded_ = My.Settings.Settings_InitData_Staff_Loaded
        config_.SettingsInitData_Staff_PathToFile_ = My.Settings.Settings_InitData_Staff_PathToFile

        config_.SettingsInitData_StaffDoctorAttr_Loaded_ = My.Settings.Settings_InitData_StaffDoctorAttr_Loaded
        config_.SettingsInitData_StaffDoctorAttr_PathToFile_ = My.Settings.Settings_InitData_StaffDoctorAttr_PathToFile

        config_.SettingsInitData_StaffNurseAttr_Loaded_ = My.Settings.Settings_InitData_StaffNurseAttr_Loaded
        config_.SettingsInitData_StaffNurseAttr_PathToFile_ = My.Settings.Settings_InitData_StaffNurseAttr_PathToFile

        config_.SettingsInitData_Structure_Loaded_ = My.Settings.Settings_InitData_Structure_Loaded
        config_.SettingsInitData_Structure_PathToFile_ = My.Settings.Settings_InitData_Structure_PathToFile

        config_.SettingsInitData_Topology_Loaded_ = My.Settings.Settings_InitData_Topology_Loaded
        config_.SettingsInitData_Topology_PathToFile_ = My.Settings.Settings_InitData_Topology_PathToFile


    End Sub


    Public Sub SaveConfigurationPersistently()

        ' 1/2. Update settings
        ' Data base settings:
        'My.Settings.SettingsDataBaseIsInitialized = config_.SettingsDataBaseIsInitialized_
        My.Settings.SettingsDataBasePath = config_.SettingsDataBasePath_

        My.Settings.SettingsAutoImportEnabled = config_.SettingsAutoImportEnabled_
        My.Settings.SettingsHospitalName = config_.SettingsHospitalName_
        My.Settings.SettingsAutoImportTimePeriodInMins = config_.SettingsAutoImportTimePeriodInMins_
        My.Settings.SettingsAutoImportFilePathWithDataForImporting = config_.SettingsAutoImportFilePathWithDataForImporting_


        My.Settings.Settings_InitData_Ward_Loaded = config_.SettingsInitData_Ward_Loaded_
        My.Settings.Settings_InitData_Ward_PathToFile = config_.SettingsInitData_Ward_PathToFile_

        My.Settings.Settings_InitData_Room_Loaded = config_.SettingsInitData_Room_Loaded_
        My.Settings.Settings_InitData_Room_PathToFile = config_.SettingsInitData_Room_PathToFile_

        My.Settings.Settings_InitData_Staff_Loaded = config_.SettingsInitData_Staff_Loaded_
        My.Settings.Settings_InitData_Staff_PathToFile = config_.SettingsInitData_Staff_PathToFile_

        My.Settings.Settings_InitData_StaffDoctorAttr_Loaded = config_.SettingsInitData_StaffDoctorAttr_Loaded_
        My.Settings.Settings_InitData_StaffDoctorAttr_PathToFile = config_.SettingsInitData_StaffDoctorAttr_PathToFile_

        My.Settings.Settings_InitData_StaffNurseAttr_Loaded = config_.SettingsInitData_StaffNurseAttr_Loaded_
        My.Settings.Settings_InitData_StaffNurseAttr_PathToFile = config_.SettingsInitData_StaffNurseAttr_PathToFile_

        My.Settings.Settings_InitData_Structure_Loaded = config_.SettingsInitData_Structure_Loaded_
        My.Settings.Settings_InitData_Structure_PathToFile = config_.SettingsInitData_Structure_PathToFile_

        My.Settings.Settings_InitData_Topology_Loaded = config_.SettingsInitData_Topology_Loaded_
        My.Settings.Settings_InitData_Topology_PathToFile = config_.SettingsInitData_Topology_PathToFile_

        ' 2/2. Save them
        My.Settings.Save()

    End Sub

    Sub Application_Started()
        authenticated_ = False
        'Dim Login As New LoginWindow
        'Login.Show()
        LoadPersistentConfiguration()

        dataReaderModule_.LaunchMonitor()
        DatabaseSqliteInterface_.SetUp(config_.SettingsDataBasePath_)
        SaveConfigurationPersistently()

        dataExporter_ = New DataExporter(DatabaseSqliteInterface_)

        Console.WriteLine("[Application] has been launched")

    End Sub

End Class
