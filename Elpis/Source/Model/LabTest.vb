

Public Class LabTest

    ' Formating:
    ' f_ - form value
    ' d_ data base value
    ' other 

    'Public id_ As UInteger
    Public d_id_ As Integer
    Public f_id_ As Integer = -1
    Public f_patient_id_ As Integer  ' forgein key of <-> Patient.f_id_

    Public f_p1_date_ As Date
    Public f_p1_result_bin_ As Integer
    Public f_p1_result_nominal_ As Integer
    Public f_p1_kind_ As String

    Public f_p2_date_ As Date
    Public f_p2_result_bin_ As Integer
    Public f_p2_result_nominal_ As Integer
    Public f_p2_kind_ As String
    'Public f_type_ As LabTestType

    Private table_name_ As String = "TEST"
    Private timeFormat_ As String = "yyyy-MM-dd HH:mm:ss"

    Function IsInitialized() As Boolean
        Return d_id_ > -1
    End Function


    Function Get_d_Id() As String
        Return d_id_.ToString
    End Function


    Function Get_f_Id() As String
        Return f_id_.ToString
    End Function

    Function Get_f_Patient_Id() As String
        Return f_patient_id_.ToString
    End Function

    Function Get_f_P1_Date() As String
        Return f_p1_date_.ToString(timeFormat_)
    End Function
    Function Get_f_P1_Result_Bin() As String
        Return f_p1_result_bin_.ToString
    End Function
    Function Get_f_P1_Result_Nominal() As String
        Return f_p1_result_nominal_.ToString
    End Function
    Function Get_f_P1_Kind() As String
        Return f_p1_kind_
    End Function

    Function Get_f_P2_Date() As String
        Return f_p2_date_.ToString(timeFormat_)
    End Function
    Function Get_f_P2_Result_Bin() As String
        Return f_p2_result_bin_.ToString
    End Function
    Function Get_f_P2_Result_Nominal() As String
        Return f_p2_result_nominal_.ToString
    End Function
    Function Get_f_P2_Kind() As String
        Return f_p2_kind_
    End Function

    Function Get_TableName() As String
        Return table_name_
    End Function

    Private cQuote As String = Chr(34)


    Function ValidateAndUpdate(
        ByVal f_id As String,
        ByVal f_p1_date As String,
        ByVal f_p1_result_bin As String,
        ByVal f_p1_result_nominal As String,
        ByVal f_p1_kind As String,
        ByVal f_p2_date As String,
        ByVal f_p2_result_bin As String,
        ByVal f_p2_result_nominal As String,
        ByVal f_p2_kind As String,
        Optional ByVal f_patient_id As String = ""
    ) As Boolean
        Dim f_id_local As Integer = -1

        Dim f_p1_date_local As Date
        Dim f_p1_result_bin_local As Integer
        Dim f_p1_result_nominal_local As Integer
        Dim f_p1_kind_local As String

        Dim f_p2_date_local As Date
        Dim f_p2_result_bin_local As Integer
        Dim f_p2_result_nominal_local As Integer
        Dim f_p2_kind_local As String

        Dim f_patient_id_local As Integer = -1 ' forgein key of <-> Patient.f_id_

        Dim validation_passed = True
        Try
            If f_id = String.Empty Then
                f_id_local = -1
                d_id_ = -1 ' IF Ward id was not set then the data shall be treated not initialized
            Else
                f_id_local = CInt(f_id)
            End If

            If f_p1_date = String.Empty Then
                f_p1_date_local = Date.MinValue
            Else
                If Not Date.TryParse(f_p1_date, f_p1_date_local) Then
                    f_p1_date_local = Date.MinValue
                End If
            End If

            If f_p1_result_bin = String.Empty Then
                f_p1_result_bin_local = -1
            Else
                f_p1_result_bin_local = CInt(f_p1_result_bin)
            End If
            If f_p1_result_nominal = String.Empty Then
                f_p1_result_nominal_local = -1
            Else
                f_p1_result_nominal_local = CInt(f_p1_result_nominal)
            End If
            f_p1_kind_local = f_p1_kind


            If f_p2_date = String.Empty Then
                f_p2_date_local = Date.MinValue
            Else
                If Not Date.TryParse(f_p2_date, f_p2_date_local) Then
                    f_p2_date_local = Date.MinValue
                End If
            End If
            If f_p2_result_bin = String.Empty Then
                f_p2_result_bin_local = -1
            Else
                f_p2_result_bin_local = CInt(f_p2_result_bin)
            End If
            If f_p2_result_nominal = String.Empty Then
                f_p2_result_nominal_local = -1
            Else
                f_p2_result_nominal_local = CInt(f_p2_result_nominal)
            End If
            f_p2_kind_local = f_p2_kind

            If f_patient_id = String.Empty Then
                f_patient_id_local = -1
            Else
                f_patient_id_local = CInt(f_patient_id)
            End If


        Catch ex As Exception
            validation_passed = False
            MsgBox("LabTest validation failed - wrong input data " & ex.Message, MsgBoxStyle.Information)
            'Toolbox.Log("Ward validation failed - wrong data " & ex.Message, MsgBoxStyle.Information) 
        End Try
        If validation_passed Then
            Update(f_id_local,
                   f_p1_date_local, f_p1_result_bin_local, f_p1_result_nominal_local, f_p1_kind,
                   f_p2_date_local, f_p2_result_bin_local, f_p2_result_nominal_local, f_p2_kind,
                   f_patient_id_local)
        End If
        Return validation_passed
    End Function


    Sub Update(
        ByVal f_id As String,
        ByVal f_p1_date As Date,
        ByVal f_p1_result_bin As Integer,
        ByVal f_p1_result_nominal As Integer,
        ByVal f_p1_kind As String,
        ByVal f_p2_date As Date,
        ByVal f_p2_result_bin As Integer,
        ByVal f_p2_result_nominal As Integer,
        ByVal f_p2_kind As String,
        Optional ByVal f_patient_id As Integer = -1
    )
        f_id_ = f_id

        f_p1_date_ = f_p1_date
        f_p1_result_bin_ = f_p1_result_bin
        f_p1_result_nominal_ = f_p1_result_nominal
        f_p1_kind_ = f_p1_kind

        f_p2_date_ = f_p2_date
        f_p2_result_bin_ = f_p2_result_bin
        f_p2_result_nominal_ = f_p2_result_nominal
        f_p2_kind_ = f_p2_kind

        f_patient_id_ = f_patient_id  ' forgein key of <-> Patient.f_id_

    End Sub

    'Sub Update(
    '    ByVal d_id As Integer,
    '    ByVal f_id As Integer,
    '    ByVal f_name As String,
    '    ByVal f_date_in As Date,
    '    ByVal f_date_out As Date,
    '    Optional ByVal f_patient_id As Integer = -1
    ')
    '    'id_ = id
    '    d_id_ = d_id
    '    f_id_ = f_id
    '    f_name_ = f_name
    '    f_date_in_ = f_date_in
    '    f_date_out_ = f_date_out
    '    f_patient_id_ = f_patient_id

    'End Sub

    Sub Clear() ' Reset class fields

        d_id_ = -1
        f_id_ = -1

        f_p1_date_ = Date.MinValue
        f_p1_result_bin_ = -1
        f_p1_result_nominal_ = -1
        f_p1_kind_ = String.Empty

        f_p2_date_ = Date.MinValue
        f_p2_result_bin_ = -1
        f_p2_result_nominal_ = -1
        f_p2_kind_ = String.Empty

        f_patient_id_ = -1  ' forgein key of <-> Patient.f_id_
    End Sub


    ' This Parse method and FormQuerry_Select must keep the same columns order
    Function Parse(ByVal s As String) As Boolean
        Dim d_id_local As String = String.Empty
        Dim f_id_local As String = String.Empty

        Dim f_p1_date_local As String = String.Empty
        Dim f_p1_result_bin_local As String = String.Empty
        Dim f_p1_result_nominal_local As String = String.Empty
        Dim f_p1_kind_local As String = String.Empty

        Dim f_p2_date_local As String = String.Empty
        Dim f_p2_result_bin_local As String = String.Empty
        Dim f_p2_result_nominal_local As String = String.Empty
        Dim f_p2_kind_local As String = String.Empty

        Dim f_patient_id_local As String = String.Empty

        Dim parts As String() = Toolbox.SplitString(s)

        d_id_local = parts(0)
        f_id_local = parts(1)
        f_patient_id_local = parts(2)

        f_p1_date_local = parts(3)
        f_p1_result_bin_local = parts(4)
        f_p1_result_nominal_local = parts(5)
        f_p1_kind_local = parts(6)

        f_p2_date_local = parts(7)
        f_p2_result_bin_local = parts(8)
        f_p2_result_nominal_local = parts(9)
        f_p2_kind_local = parts(10)

        ValidateAndUpdate(f_id_local,
                          f_p1_date_local, f_p1_result_bin_local, f_p1_result_nominal_local, f_p1_kind_local,
                          f_p2_date_local, f_p2_result_bin_local, f_p2_result_nominal_local, f_p2_kind_local,
                          f_patient_id_local)

        d_id_ = CInt(d_id_local)

    End Function

    ' This FormQuerry_Select method and Parse must keep the same columns order
    Function FormQuerry_Select() As String

        ' THE LabTest QUERRY ORDER: |                 0                 |       1          |      2           |   3     |    4          |  5                 |  6      |
        ' THE LabTest QUERRY ORDER: |                 ID                |     ID_TEST      |   PATIENT_ID     | P1_DATE | P1_RESULT_BIN | P1_RESULT_NOMINAL  | P1_KIND |
        ' THE LabTest QUERRY ORDER: | INTEGER PRIMARY KEY AUTOINCREMENT | INTEGER NOT NULL | INTEGER NOT NULL |  DATE   |  INTEGER      | INTEGER            |  TEXT   |  

        ' THE LabTest QUERRY ORDER: |   7     |    8          |  9                |  10     |
        ' THE LabTest QUERRY ORDER: | P2_DATE | P2_RESULT_BIN | P2_RESULT_NOMINAL | P2_KIND |
        ' THE LabTest QUERRY ORDER: |  DATE   |  INTEGER      |   INTEGER         |   TEXT  |  

        Dim querry As String = String.Empty
        querry = "SELECT ID, ID_TEST, PATIENT_ID, P1_DATE, P1_RESULT_BIN, P1_RESULT_NOMINAL, P1_KIND, P2_DATE, P2_RESULT_BIN, P2_RESULT_NOMINAL, P2_KIND FROM TEST"
        If f_id_ > -1 Then
            querry += " WHERE ID_TEST = " + f_id_.ToString()
        End If
        querry += ";"
        Return querry
    End Function

    Public Function FormQuerry_Insert() As String
        Dim querry As String = String.Empty
        'TODO: 
        'querry = "INSERT INTO LabTest(ID, ID_LabTest, PATIENT_ID, NUMBER, DATE_IN, DATE_OUT, NAM) VALUES (" +
        'When inserting d_id or (ID) shall be autoincremented ID.ToString & "," &

        querry = "INSERT INTO TEST (ID_TEST, PATIENT_ID, P1_DATE, P1_RESULT_BIN, P1_RESULT_NOMINAL, P1_KIND, P2_DATE, P2_RESULT_BIN, P2_RESULT_NOMINAL, P2_KIND) VALUES (" +
            f_id_.ToString & "," &
            f_patient_id_.ToString & "," &
            cQuote & f_p1_date_.ToString(timeFormat_) & cQuote & "," &
            f_p1_result_bin_.ToString & "," &
            f_p1_result_nominal_.ToString & "," &
             cQuote & f_p1_kind_ & cQuote & "," &
            cQuote & f_p2_date_.ToString(timeFormat_) & cQuote & "," &
            f_p2_result_bin_.ToString & "," &
            f_p2_result_nominal_.ToString & "," &
             cQuote & f_p2_kind_ & cQuote &
            ");"
        Return querry

    End Function

    Function FormQuerry_Update() As String ' Same as Update

        Dim querry As String = String.Empty
        ' Utworzenie querry

        ' TODO: Complete that
        ' "Type=" & quote & LabTestType.ToString() & quote & ", " &

        querry = "UPDATE TEST SET " &
         " ID_TEST = " & f_id_.ToString() & ", " &
         " PATIENT_ID = " & f_patient_id_.ToString() & ", " &
         " P1_DATE = " & cQuote & f_p1_date_.ToString(timeFormat_) & cQuote & ", " &
         " P1_RESULT_BIN = " & f_p1_result_bin_.ToString() & ", " &
         " P1_RESULT_NOMINAL = " & f_p1_result_nominal_.ToString() & ", " &
         " P1_KIND = " & cQuote & f_p1_kind_ & cQuote & ", " &
         " P2_DATE = " & cQuote & f_p2_date_.ToString(timeFormat_) & cQuote & ", " &
         " P2_RESULT_BIN = " & f_p2_result_bin_.ToString() & ", " &
         " P2_RESULT_NOMINAL = " & f_p2_result_nominal_.ToString() & ", " &
         " P2_KIND = " & cQuote & f_p2_kind_ & cQuote & " " &
         " WHERE ID = " & +d_id_.ToString &
         ";"
        Return querry
    End Function

    Function FormQuerry_Delete() As String
        Dim querry As String = String.Empty
        querry = "DELETE FROM TEST " &
        " WHERE ID = " & +d_id_.ToString & ";"
        Return querry
    End Function


    Private Shared Function FormQuerry_DropTableIfExists() As String
        Dim querry As String = String.Empty
        querry = "DROP TABLE If EXISTS TEST; "
        Return querry
    End Function

    Private Shared Function FormQuerry_CreateTableIfNotExists() As String
        Dim querry As String = String.Empty
        querry = "CREATE TABLE IF NOT EXISTS TEST ( 
                    ID INTEGER PRIMARY KEY AUTOINCREMENT,
                    ID_TEST INTEGER NOT NULL,
                    PATIENT_ID INTEGER NOT NULL,
                    P1_DATE DATE,
                    P1_RESULT_BIN INTEGER,
                    P1_RESULT_NOMINAL INTEGER,
                    P1_KIND TEXT,
                    P2_DATE DATE,
                    P2_RESULT_BIN INTEGER,
                    P2_RESULT_NOMINAL INTEGER,
                    P2_KIND TEXT 
                );"
        Return querry
    End Function

    Public Shared Function FormQuerry_DropAndCreateTable() As String
        Dim querry As String = String.Empty
        querry = FormQuerry_DropTableIfExists()
        querry += FormQuerry_CreateTableIfNotExists()
        Return querry
    End Function

End Class

'Public Class Labtest

'    Public id_ As UInteger
'    Public shortName_ As String
'    Public description_ As String
'    Public date_ As Date

'    Public type_ As String
'    Public result_ As Double

'    Sub New(ByVal id As UInteger, ByVal labtestDate As Date)
'        id_ = id
'        date_ = labtestDate
'    End Sub
'    Sub New(ByVal id As UInteger, ByVal labtestDate As Date, ByVal type As String, ByVal result As Double)
'        id_ = id
'        date_ = labtestDate
'    End Sub
'End Class
