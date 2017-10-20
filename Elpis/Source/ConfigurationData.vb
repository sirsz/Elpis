Public Class ConfigurationData

    ' Application general settings:
    Public SettingsHospitalName_ As String = "Szpital"

    ' Data base settings:
    Public SettingsDataBasePath_ As String = ""
    Public SettingsDataBaseIsInitialized_ As Boolean = False

    ' Auto import settings:
    Public SettingsAutoImportFilePathWithDataForImporting_ As String = IO.Path.GetFullPath(My.Application.Info.DirectoryPath)
    'Public SettingsAutoImportFileName_ As String = "visits.csv"
    Public SettingsAutoImportEnabled_ As Boolean = False
    Public SettingsAutoImportTimePeriodInMins_ As Integer = 60

    ' Data Init - Import the rest data:

    Public SettingsInitData_Ward_Loaded_ As Boolean = False
    Public SettingsInitData_Ward_PathToFile_ As String = String.Empty

    Public SettingsInitData_Room_Loaded_ As Boolean = False
    Public SettingsInitData_Room_PathToFile_ As String = String.Empty

    Public SettingsInitData_Staff_Loaded_ As Boolean = False
    Public SettingsInitData_Staff_PathToFile_ As String = String.Empty

    Public SettingsInitData_StaffDoctorAttr_Loaded_ As Boolean = False
    Public SettingsInitData_StaffDoctorAttr_PathToFile_ As String = String.Empty

    Public SettingsInitData_StaffNurseAttr_Loaded_ As Boolean = False
    Public SettingsInitData_StaffNurseAttr_PathToFile_ As String = String.Empty

    Public SettingsInitData_Structure_Loaded_ As Boolean = False
    Public SettingsInitData_Structure_PathToFile_ As String = String.Empty

    Public SettingsInitData_Topology_Loaded_ As Boolean = False
    Public SettingsInitData_Topology_PathToFile_ As String = String.Empty

    Public WardTypes_hashtable As New Hashtable
    ' Application internal settings (not changed in the Settings window)

    Public Function GetHashTable_Id_for_Value(ByRef pattern As String) As Integer
        GetHashTable_Id_for_Value = -1
        For index As Integer = 1 To WardTypes_hashtable.Count
            ' If index is between 5 and 7, continue
            ' with the next iteration.
            If pattern = WardTypes_hashtable.Item(index) Then
                Return index
            End If
        Next
    End Function

    Public SettingsDirectoryWhereExportedDataGoes_ As String = "siralgorytm\exported"


    Public Sub New()
        ' Note: this initalization must match equal to the ComboBox initalization. Otherwise it will not works properly.

        WardTypes_hashtable.Add(0, "Oddziały Niezabiegowe - Dorosłych")
        WardTypes_hashtable.Add(1, "Oddziały Zabiegowe - Dorosłych")
        WardTypes_hashtable.Add(2, "Oddziały Niezabiegowe - Dzieci")
        WardTypes_hashtable.Add(3, "Anestezjologii i intesywnejterapii - Dorosłych")
        WardTypes_hashtable.Add(4, "Psychiatryczny")
        WardTypes_hashtable.Add(5, "Położniczy/ginekologiczny")
        WardTypes_hashtable.Add(6, "Rehabilitacyjny")
        WardTypes_hashtable.Add(7, "Chirurgii urazowo")
        WardTypes_hashtable.Add(8, "Neonatologiczny")
        WardTypes_hashtable.Add(9, "Hematologiczny, onkologiczny")
        WardTypes_hashtable.Add(10, "Chorób zakaźnych")
        WardTypes_hashtable.Add(11, "Oddziały Zabiegowe - Dzieci")
        WardTypes_hashtable.Add(12, "Inny")

        Toolbox.Log(DateTime.Now.ToString("[yyyy/MM/dd HH:mm:ss]: ") & "ConfigurationData container has been created")
    End Sub


End Class
