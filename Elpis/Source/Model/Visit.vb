


Imports System.Collections.Generic

Public Enum VisitType
    Undefined
    Type1
    Type2
    Type3
End Enum


Public Class Visit

    ' Formating:
    ' f_ - form value
    ' d_ data base value
    ' other 

    'Public id_ As UInteger
    Public d_id_ As Integer
    Public f_id_ As Integer
    Public f_patient_id_ As Integer  ' forgein key of <-> Patient.f_id_
    Public f_name_ As String
    Public f_date_in_ As Date
    Public f_date_out_ As Date
    Public f_ward_id_ As Integer ' forgein key of <-> Ward.f_id_
    'Public f_type_ As VisitType

    Private table_name_ As String = "VISIT"
    Private timeFormat_ As String = "yyyy-MM-dd HH:mm:ss"

    Function IsInitialized() As Boolean
        Return d_id_ > -1
    End Function


    Function Get_d_Id() As String
        Return d_id_.ToString
    End Function

    Function Get_f_Patient_Id() As String
        Return f_patient_id_.ToString
    End Function

    Function Get_f_Id() As String
        Return f_id_.ToString
    End Function

    Function Get_f_Name() As String
        Return f_name_
    End Function

    Function Get_f_Date_In() As String
        Return f_date_in_.ToString(timeFormat_)
    End Function

    Function Get_f_Date_Out() As String
        Return f_date_out_.ToString(timeFormat_)
    End Function

    Function Get_TableName() As String
        Return table_name_
    End Function

    Function Get_Ward_Id() As String
        Return f_ward_id_
    End Function

    Private cQuote As String = Chr(34)


    Function ValidateAndUpdate(
        ByVal f_id As String,
        ByVal f_name As String,
        ByVal f_date_in As String,
        ByVal f_date_out As String,
        Optional ByVal f_ward_id As String = "",
        Optional ByVal f_patient_id As String = ""
    ) As Boolean
        Dim f_id_local As Integer = -1
        Dim f_name_local As String = ""
        Dim f_date_in_local As Date
        Dim f_date_out_local As Date
        Dim f_ward_id_local As Integer = -1
        Dim f_patient_id_local As Integer = -1

        Dim validation_passed = True
        Try
            If f_id = String.Empty Then
                f_id_local = -1
                d_id_ = -1 ' IF Ward id was not set then the data shall be treated not initialized
            Else
                f_id_local = CInt(f_id)
            End If
            f_name_local = f_name

            If f_date_in = String.Empty Then
                f_date_in_local = Date.MinValue
            Else
                If Not Date.TryParse(f_date_in, f_date_in_local) Then
                    f_date_in_local = Date.MinValue
                End If
            End If
            If f_date_out = String.Empty Then
                f_date_out_local = Date.MinValue
            Else
                If Not Date.TryParse(f_date_out, f_date_out_local) Then
                    f_date_out_local = Date.MinValue
                End If
            End If
            If f_ward_id = String.Empty Then
                f_ward_id_local = -1
            Else
                f_ward_id_local = CInt(f_ward_id)
            End If
            If f_patient_id = String.Empty Then
                f_patient_id_local = -1
            Else
                f_patient_id_local = CInt(f_patient_id)
            End If

        Catch ex As Exception
            validation_passed = False
            MsgBox("Visit validation failed - wrong data " & ex.Message, MsgBoxStyle.Information)
            'Toolbox.Log("Ward validation failed - wrong data " & ex.Message, MsgBoxStyle.Information)
        End Try
        If validation_passed Then
            Update(f_id_local, f_name_local, f_date_in_local, f_date_out_local, f_ward_id_local, f_patient_id_local)
        End If
        Return validation_passed
    End Function


    Sub Update(
        ByVal f_id As Integer,
        ByVal f_name As String,
        ByVal f_date_in As Date,
        ByVal f_date_out As Date,
        Optional ByVal f_ward_id As Integer = -1,
        Optional ByVal f_patient_id As Integer = -1
    )
        f_id_ = f_id
        f_name_ = f_name
        f_date_in_ = f_date_in
        f_date_out_ = f_date_out
        f_ward_id_ = f_ward_id
        f_patient_id_ = f_patient_id

    End Sub

    Sub Update(
        ByVal d_id As Integer,
        ByVal f_id As Integer,
        ByVal f_name As String,
        ByVal f_date_in As Date,
        ByVal f_date_out As Date,
        Optional ByVal f_ward_id As Integer = -1,
        Optional ByVal f_patient_id As Integer = -1
    )
        'id_ = id
        d_id_ = d_id
        f_id_ = f_id
        f_name_ = f_name
        f_date_in_ = f_date_in
        f_date_out_ = f_date_out
        f_ward_id_ = f_ward_id
        f_patient_id_ = f_patient_id

    End Sub

    Sub Clear() ' Reset class fields
        'id_ = 0
        d_id_ = -1
        f_name_ = String.Empty
        f_id_ = -1
        f_date_in_ = Date.MinValue
        f_date_out_ = Date.MinValue
        f_ward_id_ = -1
        f_patient_id_ = -1
    End Sub


    ' This Parse method and FormQuerry_Select must keep the same columns order
    Function Parse(ByVal s As String) As Boolean
        Dim d_id_local As String
        Dim f_id_local As String
        Dim f_name_local As String
        Dim f_date_in_local As String
        Dim f_date_out_local As String
        Dim f_ward_id_local As String
        Dim f_patient_id_local As String

        Dim parts As String() = Toolbox.SplitString(s)
        d_id_local = parts(0)
        f_id_local = parts(1)
        f_patient_id_local = parts(2)
        f_ward_id_local = parts(3)
        f_date_in_local = parts(4)
        f_date_out_local = parts(5)
        f_name_local = parts(6)

        ValidateAndUpdate(f_id_local, f_name_local, f_date_in_local, f_date_out_local, f_ward_id_local, f_patient_id_local)
        d_id_ = CInt(d_id_local)

    End Function

    ' This FormQuerry_Select method and Parse must keep the same columns order
    Function FormQuerry_Select() As String

        ' THE VISIT QUERRY ORDER: |                 0                 |     1            |      2          |      3           |    4          |    5     |  6    |
        ' THE VISIT QUERRY ORDER: |                 ID                |     ID_VISIT     |   PATIENT_ID    |   WARD_ID        |   DATE_IN     | DATE_OUT | NAME  |
        ' THE VISIT QUERRY ORDER: | INTEGER PRIMARY KEY AUTOINCREMENT | INTEGER NOT NULL | INTEGER NOT NULL| INTEGER NOT NULL | DATE NOT NULL |  DATE    | TEXT  |

        Dim querry As String = String.Empty
        querry = "SELECT ID, ID_VISIT, PATIENT_ID, WARD_ID, DATE_IN, DATE_OUT, NAME FROM VISIT"
        If f_id_ > -1 Then
            querry += " WHERE ID_VISIT = " + f_id_.ToString()
        End If
        querry += ";"
        Return querry
    End Function

    ' This FormQuerry_Select method and Parse must keep the same columns order
    ' Defined in email 2017-07-06: 
    ' data_przyjecia, data_wypisu, id_pacjenta, id_oddzialu
    Function FormQuerry_SelectSpecialForExport() As String

        ' THE VISIT QUERRY ORDER: |    0          |    1     |     2             |    3             |
        ' THE VISIT QUERRY ORDER: |   DATE_IN     | DATE_OUT |  PATIENT_ID       |  WARD_ID         |
        ' THE VISIT QUERRY ORDER: | DATE NOT NULL |  DATE    | INTEGER NOT NULL  | INTEGER NOT NULL |

        Dim querry As String = String.Empty
        'querry = "SELECT DATE_IN, DATE_OUT, PATIENT_ID, WARD_ID FROM VISIT"
        querry = "SELECT STRFTIME('%Y-%m-%d',DATE_IN), STRFTIME('%Y-%m-%d', DATE_OUT), PATIENT_ID, WARD_ID FROM VISIT"
        If f_id_ > -1 Then
            querry += " WHERE ID_VISIT = " + f_id_.ToString()
        End If
        querry += ";"
        Return querry
    End Function

    ' This FormQuerry_Select method and Parse must keep the same columns order
    ' Defined in email 2017-07-06: 
    ' data_przyjecia, data_wypisu, id_pacjenta, id_oddzialu
    Function FormQuerry_SelectSpecialForExport_Header() As String
        Dim header As String = String.Empty
        header = "data_przyjecia;data_wypisu;id_pacjenta;id_oddzialu;"
        Return header
    End Function

    Public Function FormQuerry_Insert() As String
        Dim querry As String = String.Empty
        'TODO: 
        'querry = "INSERT INTO VISIT(ID, ID_VISIT, PATIENT_ID, NUMBER, DATE_IN, DATE_OUT, NAM) VALUES (" +
        'When inserting d_id or (ID) shall be autoincremented ID.ToString & "," &

        querry = "INSERT INTO VISIT (ID_VISIT, PATIENT_ID, WARD_ID, DATE_IN, DATE_OUT, NAME) VALUES (" +
            f_id_.ToString & "," &
            f_patient_id_.ToString & "," &
            f_ward_id_.ToString & "," &
            cQuote & f_date_in_.ToString(timeFormat_) & cQuote & "," &
            cQuote & f_date_out_.ToString(timeFormat_) & cQuote & "," &
            cQuote & f_name_ & cQuote &
            ");"
        Return querry

    End Function

    Function FormQuerry_Update() As String ' Same as Update

        Dim querry As String = String.Empty
        ' Utworzenie querry

        ' TODO: Complete that
        ' "Type=" & quote & VisitType.ToString() & quote & ", " &

        querry = "UPDATE VISIT SET " &
         " ID_VISIT = " & f_id_.ToString() & ", " &
         " PATIENT_ID = " & f_patient_id_.ToString() & ", " &
         " WARD_ID = " & f_ward_id_.ToString() & ", " &
         " DATE_IN = " & cQuote & f_date_in_.ToString(timeFormat_) & cQuote & ", " &
         " DATE_OUT = " & cQuote & f_date_out_.ToString(timeFormat_) & cQuote & ", " &
         " NAME = " & cQuote & f_name_ & cQuote & " " &
         " WHERE ID = " & +d_id_.ToString &
         ";"
        Return querry
    End Function

    Function FormQuerry_Delete() As String
        Dim querry As String = String.Empty
        querry = "DELETE FROM VISIT " &
        " WHERE ID = " & +d_id_.ToString & ";"
        Return querry
    End Function


    Private Shared Function FormQuerry_DropTableIfExists() As String
        Dim querry As String = String.Empty
        querry = "DROP TABLE If EXISTS VISIT; "
        Return querry
    End Function

    Private Shared Function FormQuerry_CreateTableIfNotExists() As String
        Dim querry As String = String.Empty
        querry = "CREATE TABLE IF NOT EXISTS VISIT ( 
                    ID INTEGER PRIMARY KEY AUTOINCREMENT,
                    ID_VISIT INTEGER NOT NULL,
                    PATIENT_ID INTEGER NOT NULL,
                    WARD_ID INTEGER NOT NULL,
                    DATE_IN  DATE NOT NULL,
                    DATE_OUT DATE,
                    NAME TEXT 
                ); "
        Return querry
    End Function

    Public Shared Function FormQuerry_DropAndCreateTable() As String
        Dim querry As String = String.Empty
        querry = FormQuerry_DropTableIfExists()
        querry += FormQuerry_CreateTableIfNotExists()
        Return querry
    End Function

End Class

'Public Class Visit
'    Public id_ As UInteger
'    Public dateOfIncoming_ As Date
'    Public dateOfLeaving_ As Date
'    Public placeId_ As UInteger

'    Private patient_ As Patient
'    Private labtest_ As Labtest
'    Sub New(ByVal id As UInteger, ByVal dateOfIncoming As Date)
'        id_ = id
'        dateOfIncoming_ = dateOfIncoming
'    End Sub
'    Sub New(ByVal id As UInteger, ByVal dateOfIncoming As Date, ByVal placeId As UInteger)
'        id_ = id
'        dateOfIncoming_ = dateOfIncoming
'        placeId_ = placeId
'    End Sub

'    Sub SetDateOfLeaving(ByVal leavingDate As Date)
'        dateOfLeaving_ = leavingDate
'    End Sub


'    Sub SetPatient(ByVal patient As Patient)
'        patient_ = patient
'    End Sub

'    Sub AddLabtest(ByVal labtest As Labtest)
'        labtest_ = labtest
'    End Sub

'End Class
