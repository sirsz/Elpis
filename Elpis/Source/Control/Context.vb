Imports System.Collections.ArrayList

Public Class Context

    Public contextID_ As Integer
    Public Enum TabSelected
        None = 0
        Hospital = 1
        Ward = 2
        Room = 3
        Staff = 4
        Patient = 5
        Visit = 6
        LabTest = 7
        Duty = 8
    End Enum

    Public TabSelected_ As TabSelected = TabSelected.None

    Public currHospital_ As Hospital

    Public currWard_ As New Ward
    Public currRoom_ As New Room

    Public currHospitalStaff_ As New HospitalStaff
    Public currPatient_ As New Patient ' Unused

    Public currVisit_ As New Visit
    Public currLabTest_ As New LabTest


    Public currWard_FromIsInitalized As Boolean

    Public patientList As New ArrayList

    Public currDuty_ As New Duty


    Public Sub New()
        contextID_ = -1
        currWard_FromIsInitalized = False
    End Sub

    Public Sub SetContext(ByVal newHospital As Hospital,
                           ByVal newPatient As Patient,
                         ByVal newWard As Ward,
                           ByVal newVisit As Visit,
                          ByVal newLabTest As LabTest,
                          ByVal newDuty As Duty)
        currHospital_ = newHospital
        currPatient_ = newPatient
        currWard_ = newWard
        currVisit_ = newVisit
        currLabTest_ = newLabTest
        currDuty_ = newDuty

    End Sub
    Public Sub UpdateHospital(ByVal newHospital As Hospital)
        currHospital_ = newHospital
    End Sub
    Public Sub UpdatePatient(ByVal newPatient As Patient)
        currPatient_ = newPatient
    End Sub
    Public Sub UpdateHospital(ByVal newWard As Ward)
        currWard_ = newWard
    End Sub
    Public Sub UpdatePatient(ByVal newVisit As Visit)
        currVisit_ = newVisit
    End Sub
    Public Sub UpdatePatient(ByVal newLabTest As Labtest)
        currLabTest_ = newLabTest
    End Sub
    Public Sub UpdateDuty(ByVal newDuty As Duty)
        currDuty_ = newDuty
    End Sub

End Class
