#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include <cstdarg>

// Game options and requests
bool isGodMode = false;
bool isCompactMode = true;
bool isPortableSymbolSetUsed = true;
bool isAutomaticPlacement = true;

const char * godModeSelectionRequest = "God Mode isn't fun, but it has its own pros.\nSo, do you want to use God Mode?";
const char * symbolSetSelectionRequestPrompt = "Non portable symbol set might look wrong.\nDo you want to use portable symbol set?";
const char * compactModeSelectionRequest = "Extended mode may not fit in your console window.\nDo you want to use compact mode?";
const char * shipPlacementModeSelectionRequestPrompt = "Do you want to use automatic ship placement?";

const char * shipTilePlacementConfirmation = "Do you confirm ship tile placement?";
const char * shipPlacementConfirmation = "Do you confirm ship placement?";

// Letter coordinate array option
const char letterOfFirstRow = 'a';

// Game Board options
const int gameBoardSize = 10;
const int gameBoardShipsCount = (1 + 2 + 3 + 4);

// Representation options
const int cellRepresentationLengthCompactMode = 2;
const int cellRepresentationLengthExtendedMode = 4;

const char * playerCaptionBase = "PLAYER";
const char * playerCaptionGodeModeSuffix = " (GOD MODE)";
const char * computerCaption = "COMPUTER";

const char * legendCaption = "\nField markers which are used in game:";
const char * legendEmptyField = "\n<%c> - Game Board Field is not occupied (Computer's Ship Tiles are not shown).";
const char * legendHitField = "\n<%c> - Game Board Field was hit.";
const char * legendFieldUnavailable = "\n<%c> - Game Board Field can not be used as Ship Tile.";
const char * legendShipNotFinished = "\n<%c> - Game Board Field used as Ship Tile of not finished ship (during manual placement).";
const char * legendShipNormal = "\n<%c> - Game Board Field is occupied by a normal Ship Tile.";
const char * legendShipDamanged = "\n<%c> - Game Board Field is occupied by a damaged Ship Tile.";
const char * legendShipDestroyed = "\n<%c> - Game Board Field is occupied by a destroyed Ship Tile.";

// Game Board enumerations
enum GameBoardFieldTypes
{
    FIELD_TYPE_EMPTY = 0,
    FIELD_TYPE_UNAVAILABLE,
    FILED_TYPE_SHIP_NOT_FINISHED,
    FIELD_TYPE_SHIP_NORMAL,
    FIELD_TYPE_SHIP_DAMAGED,
    FIELD_TYPE_SHIP_DESTROYED,
    FIELD_TYPE_HIT
};

enum GameBaordShipLengths
{
    SHIP_LENGTH_1_LINE = 1,
    SHIP_LENGTH_2_LINE,
    SHIP_LENGTH_3_LINE,
    SHIP_LENGTH_4_LINE
};

// Representation enumerations
enum BoardPortableGraphicSymbols
{
    SYMBOL_VERTICAL_SEPARATOR = '|',
    SYMBOL_HORIZONTAL_SEPARATOR = '-',
    SYMBOL_SPACE = ' ',

    SYMBOL_UNAVAILABLE_FIELD = ':',
    SYMBOL_SHIP_NOT_FINISHED = '@',
    SYMBOL_SHIP_NORMAL_FIELD = '#',
    SYMBOL_SHIP_DAMAGED_FIELD = 'X',
    SYMBOL_SHIP_DESTROYED_FIELD = '~',
    SYMBOL_HIT_FIELD = '*'
};

enum BoardNonPortableGraphicElements
{
    NON_PORTABLE_CORNER_LEFT_UPPER = 201,
    NON_PORTABLE_CORNER_RIGHT_UPPER = 187,
    NON_PORTABLE_CORNER_LEFT_LOWER = 200,
    NON_PORTABLE_CORNER_RIGHT_LOWER = 188,

    NON_PORTABLE_OUTER_UPPER_LOWER_EDGE_TILE = 205,

    NON_PORTABLE_OUTER_UPPER_EDGE_DELIMETER = 216,
    NON_PORTABLE_OUTER_LOWER_EDGE_DELIMETER = 207,

    NON_PORTABLE_OUTER_LEFT_RIGHT_EDGE_TILE = 186,

    NON_PORTABLE_OUTER_LEFT_EDGE_DELIMETER = 215,
    NON_PORTABLE_OUTER_RIGHT_EDGE_DELIMETER = 182,

    NON_PORTABLE_INNER_VERTICAL_DELIMETER = 179,
    NON_PORTABLE_INNER_HORIZONTAL_DELIMETER = 196,
    NON_PORTABLE_INNER_CROSS = 197,

    NON_PORTABLE_EMPTY_FIELD = SYMBOL_SPACE,
    NON_PORTABLE_UNAVAILABLE_FIELD = 176,
    NON_PORTABLE_SHIP_NOT_FINISHED = 251,
    NON_PORTABLE_SHIP_NORMAL_FIELD = 254,
    NON_PORTABLE_SHIP_DAMAGED_FIELD = 253,
    NON_PORTABLE_SHIP_DESTROYED_FIELD = 229,
    NON_PORTABLE_HIT_FIELD = 249
};

enum BoardPortableGraphicElements
{
    PORTABLE_CORNER_LEFT_UPPER = SYMBOL_VERTICAL_SEPARATOR,
    PORTABLE_CORNER_RIGHT_UPPER = SYMBOL_VERTICAL_SEPARATOR,
    PORTABLE_CORNER_LEFT_LOWER = SYMBOL_VERTICAL_SEPARATOR,
    PORTABLE_CORNER_RIGHT_LOWER = SYMBOL_VERTICAL_SEPARATOR,

    PORTABLE_OUTER_UPPER_LOWER_EDGE_TILE = SYMBOL_HORIZONTAL_SEPARATOR,

    PORTABLE_OUTER_UPPER_EDGE_DELIMETER = SYMBOL_VERTICAL_SEPARATOR,
    PORTABLE_OUTER_LOWER_EDGE_DELIMETER = SYMBOL_VERTICAL_SEPARATOR,

    PORTABLE_OUTER_LEFT_RIGHT_EDGE_TILE = SYMBOL_VERTICAL_SEPARATOR,

    PORTABLE_OUTER_LEFT_EDGE_DELIMETER = SYMBOL_VERTICAL_SEPARATOR,
    PORTABLE_OUTER_RIGHT_EDGE_DELIMETER = SYMBOL_VERTICAL_SEPARATOR,

    PORTABLE_INNER_VERTICAL_DELIMETER = SYMBOL_VERTICAL_SEPARATOR,
    PORTABLE_INNER_HORIZONTAL_DELIMETER = SYMBOL_HORIZONTAL_SEPARATOR,
    PORTABLE_INNER_CROSS = SYMBOL_VERTICAL_SEPARATOR,

    PORTABLE_EMPTY_FIELD = SYMBOL_SPACE,
    PORTABLE_UNAVAILABLE_FIELD = SYMBOL_UNAVAILABLE_FIELD,
    PORTABLE_SHIP_NOT_FINISHED = SYMBOL_SHIP_NOT_FINISHED,
    PORTABLE_SHIP_NORMAL_FIELD = SYMBOL_SHIP_NORMAL_FIELD,
    PORTABLE_SHIP_DAMAGED_FIELD = SYMBOL_SHIP_DAMAGED_FIELD,
    PORTABLE_SHIP_DESTROYED_FIELD = SYMBOL_SHIP_DESTROYED_FIELD,
    PORTABLE_HIT_FIELD = SYMBOL_HIT_FIELD
};

// Game Board structures
struct GameBoardShipTileCoordinates
{
    int row;
    int cell;
};

struct GameBoardShip
{
    GameBaordShipLengths shipLength;
    GameBoardShipTileCoordinates * coordinates;
    bool isDestroyed;
};

struct GameBoardField
{
    GameBoardFieldTypes fieldType;
    GameBoardShip * gameBoardShip;
};

// Representation structures
struct CellPaterns
{
    char * numerationCell;
    char * characterEdgeLeft;
    char * characterEdgeRight;
    char * fieldTileSymbol;
};

struct BoardGraphicElements
{
    char cornerLeftUpper;
    char cornerRightUpper;
    char cornerLeftLower;
    char cornerRightLower;

    char outerUpperLowerEdgeTile;

    char outerUpperEdgeDelimeter;
    char outerLowerEdgeDelimeter;

    char outerLeftRightEdgeTile;

    char outerLeftEdgeDelimeter;
    char outerRightEdgeDelimeter;

    char innerVerticalDelimeter;
    char innerHorizontalDelimeter;
    char innerCross;

    char emptyField;
    char unavailableField;
    char shipNotFinished;
    char shipNormalField;
    char shipDamagedField;
    char shipDestroyedField;
    char hitField;
};

// Letter coordinate array
char * rowCoordinates = nullptr;

// Representation components
char * playerCaption = nullptr;

int cellRepresentationLength;
int cellRepresentationBufferLength;
char * cellRepresentationBuffer = nullptr;

char * upperFrame = nullptr;
char * lowerFrame = nullptr;

int distanceBetweenPLayersGameBoardAndDelemeter;
int distanceBetweenGameBoards;
int crossBoardSeparatorPosition;

int gameBoardRepresentationCellCount;
int boardRepresentationRowsCount;
int boardRepresentationRowLength;

CellPaterns cellPaterns;
BoardGraphicElements boardGraphicElements;

char ** boardRepresentation = nullptr;

//Game Board components
GameBoardShipTileCoordinates randomCoordinates;

int possibleCoordinatesCount;
bool * usedCoordinates = nullptr;

GameBoardField ** playerGameBoard = nullptr;
GameBoardShip * playerGameBoardShips = nullptr;
int playerDestroyedShipsCount;

GameBoardField ** computerGameBoard = nullptr;
GameBoardShip * computerGameBoardShips = nullptr;
int computerDestroyedShipsCount;

const int maximumAmountOfShootingPointInAgenda = 4;
GameBoardShipTileCoordinates shootingAgenda[maximumAmountOfShootingPointInAgenda];
GameBoardShipTileCoordinates currentTilesHit[maximumAmountOfShootingPointInAgenda];

//Clear input buffer
void cleanStdin()
{
    char c;
    while (((c = getchar()) != '\n') && (c != EOF) && (c != '\0'));
};

bool lineYesOrNoRequest(const char* inPromptLine)
{
    char input;
    bool result = false;
    do
    {
        printf("\n%s (y/n): ", inPromptLine);
        input = toupper(getchar());
        cleanStdin();
    } while ((input != 'Y') && (input != 'N'));
    switch (input)
    {
    case 'Y':
        result = true;
        break;
    case 'N':
    default:
        result = false;
        break;
    }
    return result;
};

GameBoardShipTileCoordinates lineCoordinatesRequest()
{
    GameBoardShipTileCoordinates result;
    result.row = -1;
    result.cell = -1;
    const int inputBufferLength = 4;
    char * inputBuffer = (char *)calloc(inputBufferLength, sizeof(char));
    do
    {
        memset(inputBuffer, ' ', inputBufferLength);
        printf("\nEnter 'x' if you want to exit the game.");
        printf("\nPlease, enter coordinate [%c..%c][0..9]: ", rowCoordinates[0], rowCoordinates[(gameBoardSize - 1)]);
        scanf_s("%s", inputBuffer, (inputBufferLength-1));
        if (toupper(inputBuffer[0]) == 'X')
        {
            exit(0);
        }
        result.row = inputBuffer[0] - rowCoordinates[0];
        result.cell = inputBuffer[1] - '0';
        cleanStdin();
    } while ((result.row<0) || (result.row>(gameBoardSize - 1)) || (result.cell<0) || (result.cell>(gameBoardSize - 1)));
    free(inputBuffer);
    return result;
}

int getRowPosition(int inCell, bool inIsForPlayer)
{
    int gameBoardShift = inIsForPlayer ? 0 : (distanceBetweenGameBoards + cellRepresentationLength * gameBoardRepresentationCellCount);
    int gameBoardCellShift = (inCell * cellRepresentationLength);
    return (gameBoardShift + gameBoardCellShift);
};

void initializeCellPaterns()
{
    cellPaterns.numerationCell = isCompactMode ? "%1d%c" : " %1d %c";
    cellPaterns.characterEdgeLeft = isCompactMode ? "%c%c" : " %c %c";
    cellPaterns.characterEdgeRight = isCompactMode ? "%2c" : "%4c";
    cellPaterns.fieldTileSymbol = isCompactMode ? "%c" : " %c ";
};

void initializeBoardGraphicElements()
{
    boardGraphicElements.cornerLeftUpper = (char)isPortableSymbolSetUsed ? PORTABLE_CORNER_LEFT_UPPER : NON_PORTABLE_CORNER_LEFT_UPPER;
    boardGraphicElements.cornerRightUpper = (char)isPortableSymbolSetUsed ? PORTABLE_CORNER_RIGHT_UPPER : NON_PORTABLE_CORNER_RIGHT_UPPER;
    boardGraphicElements.cornerLeftLower = (char)isPortableSymbolSetUsed ? PORTABLE_CORNER_LEFT_LOWER : NON_PORTABLE_CORNER_LEFT_LOWER;
    boardGraphicElements.cornerRightLower = (char)isPortableSymbolSetUsed ? PORTABLE_CORNER_RIGHT_LOWER : NON_PORTABLE_CORNER_RIGHT_LOWER;

    boardGraphicElements.outerUpperLowerEdgeTile = (char)isPortableSymbolSetUsed ? PORTABLE_OUTER_UPPER_LOWER_EDGE_TILE : NON_PORTABLE_OUTER_UPPER_LOWER_EDGE_TILE;

    boardGraphicElements.outerUpperEdgeDelimeter = (char)isPortableSymbolSetUsed ? PORTABLE_OUTER_UPPER_EDGE_DELIMETER : NON_PORTABLE_OUTER_UPPER_EDGE_DELIMETER;
    boardGraphicElements.outerLowerEdgeDelimeter = (char)isPortableSymbolSetUsed ? PORTABLE_OUTER_LOWER_EDGE_DELIMETER : NON_PORTABLE_OUTER_LOWER_EDGE_DELIMETER;

    boardGraphicElements.outerLeftRightEdgeTile = (char)isPortableSymbolSetUsed ? PORTABLE_OUTER_LEFT_RIGHT_EDGE_TILE : NON_PORTABLE_OUTER_LEFT_RIGHT_EDGE_TILE;

    boardGraphicElements.outerLeftEdgeDelimeter = (char)isPortableSymbolSetUsed ? PORTABLE_OUTER_LEFT_EDGE_DELIMETER : NON_PORTABLE_OUTER_LEFT_EDGE_DELIMETER;
    boardGraphicElements.outerRightEdgeDelimeter = (char)isPortableSymbolSetUsed ? PORTABLE_OUTER_RIGHT_EDGE_DELIMETER : NON_PORTABLE_OUTER_RIGHT_EDGE_DELIMETER;

    boardGraphicElements.innerVerticalDelimeter = (char)isPortableSymbolSetUsed ? PORTABLE_INNER_VERTICAL_DELIMETER : NON_PORTABLE_INNER_VERTICAL_DELIMETER;
    boardGraphicElements.innerHorizontalDelimeter = (char)isPortableSymbolSetUsed ? PORTABLE_INNER_HORIZONTAL_DELIMETER : NON_PORTABLE_INNER_HORIZONTAL_DELIMETER;
    boardGraphicElements.innerCross = (char)isPortableSymbolSetUsed ? PORTABLE_INNER_CROSS : NON_PORTABLE_INNER_CROSS;

    boardGraphicElements.emptyField = (char)isPortableSymbolSetUsed ? PORTABLE_EMPTY_FIELD : NON_PORTABLE_EMPTY_FIELD;
    boardGraphicElements.unavailableField = (char)isPortableSymbolSetUsed ? PORTABLE_UNAVAILABLE_FIELD : NON_PORTABLE_UNAVAILABLE_FIELD;
    boardGraphicElements.shipNotFinished = (char)isPortableSymbolSetUsed ? PORTABLE_SHIP_NOT_FINISHED : NON_PORTABLE_SHIP_NOT_FINISHED;
    boardGraphicElements.shipNormalField = (char)isPortableSymbolSetUsed ? PORTABLE_SHIP_NORMAL_FIELD : NON_PORTABLE_SHIP_NORMAL_FIELD;
    boardGraphicElements.shipDamagedField = (char)isPortableSymbolSetUsed ? PORTABLE_SHIP_DAMAGED_FIELD : NON_PORTABLE_SHIP_DAMAGED_FIELD;
    boardGraphicElements.shipDestroyedField = (char)isPortableSymbolSetUsed ? PORTABLE_SHIP_DESTROYED_FIELD : NON_PORTABLE_SHIP_DESTROYED_FIELD;
    boardGraphicElements.hitField = (char)isPortableSymbolSetUsed ? PORTABLE_HIT_FIELD : NON_PORTABLE_HIT_FIELD;
};

void setCellInstance(int inRow, int inCell, bool inIsForPlayer, int inBufferLength)
{
    memcpy(&boardRepresentation[inRow][getRowPosition(inCell, inIsForPlayer)], cellRepresentationBuffer, inBufferLength);
}

void setCellInstance(int inRow, int inCell, bool inIsForPlayer)
{
    setCellInstance(inRow, inCell, inIsForPlayer, cellRepresentationLength);
};

void setCellInstance(int inRow, int inCell)
{
    setCellInstance(inRow, inCell, true);
    setCellInstance(inRow, inCell, false);
};

void initializeFrameRow(int inRow, char inCellInnerTile, int inCellDelimeter)
{
    memset(cellRepresentationBuffer, inCellInnerTile, (cellRepresentationLength - 1));
    cellRepresentationBuffer[(cellRepresentationLength - 1)] = inCellDelimeter;
    for (int cell = 1; cell < (gameBoardRepresentationCellCount - 1); ++cell)
    {
        setCellInstance(inRow, cell);
    }
};

void setNumerationCellInstance(int inCell, char inDelimeter)
{
    sprintf_s(cellRepresentationBuffer, cellRepresentationBufferLength, cellPaterns.numerationCell, inCell, inDelimeter);
    setCellInstance(1, (inCell + 1));
};

void setCharacterEdgeCellInstance(int inRow, int inSymbolShift)
{
    sprintf_s(cellRepresentationBuffer, cellRepresentationBufferLength, cellPaterns.characterEdgeLeft, rowCoordinates[inSymbolShift], boardGraphicElements.outerLeftRightEdgeTile);
    setCellInstance(inRow, 0);
    sprintf_s(cellRepresentationBuffer, cellRepresentationBufferLength, cellPaterns.characterEdgeRight, boardGraphicElements.outerLeftRightEdgeTile);
    setCellInstance(inRow, (gameBoardRepresentationCellCount - 1));
};

void setEdgeCellInstance(int inRow)
{
    memset(cellRepresentationBuffer, boardGraphicElements.innerHorizontalDelimeter, (cellRepresentationLength - 1));
    cellRepresentationBuffer[(cellRepresentationLength - 1)] = boardGraphicElements.outerLeftEdgeDelimeter;
    setCellInstance(inRow, 0);
    cellRepresentationBuffer[(cellRepresentationLength - 1)] = boardGraphicElements.outerRightEdgeDelimeter;
    setCellInstance(inRow, (gameBoardRepresentationCellCount - 1));
};

void setCornerEdgeInstance()
{
    memset(cellRepresentationBuffer, SYMBOL_SPACE, (cellRepresentationLength - 1));
    cellRepresentationBuffer[(cellRepresentationLength - 1)] = boardGraphicElements.cornerLeftUpper;
    setCellInstance(2, 0);
    cellRepresentationBuffer[(cellRepresentationLength - 1)] = boardGraphicElements.cornerLeftLower;
    setCellInstance((boardRepresentationRowsCount - 1), 0);

    memset(cellRepresentationBuffer, boardGraphicElements.outerUpperLowerEdgeTile, (cellRepresentationLength - 1));
    cellRepresentationBuffer[(cellRepresentationLength - 1)] = boardGraphicElements.cornerRightUpper;
    setCellInstance(2, (gameBoardRepresentationCellCount - 1));
    cellRepresentationBuffer[(cellRepresentationLength - 1)] = boardGraphicElements.cornerRightLower;
    setCellInstance((boardRepresentationRowsCount - 1), (gameBoardRepresentationCellCount - 1));
};

void setFieldTypeCellRepresentationInstance(int inRow, int inCell, bool inIsForPlayer, char inTileSymbol)
{
    sprintf_s(cellRepresentationBuffer, cellRepresentationBufferLength, cellPaterns.fieldTileSymbol, inTileSymbol);
    setCellInstance((2 * inRow + 3), (inCell + 1), inIsForPlayer, (cellRepresentationLength - 1));
}

void initializeBoardRepresentationFrame()
{
    setCornerEdgeInstance();

    for (int cell = 0; cell < (gameBoardSize - 1); ++cell)
    {
        setNumerationCellInstance(cell, boardGraphicElements.innerVerticalDelimeter);
    }
    setNumerationCellInstance((gameBoardSize - 1), SYMBOL_SPACE);

    for (int row = 3; row < (boardRepresentationRowsCount - 1); ++row)
    {
        (row % 2) ? setCharacterEdgeCellInstance(row, ((row - 3) / 2)) : setEdgeCellInstance(row);
    }

    initializeFrameRow(2, boardGraphicElements.outerUpperLowerEdgeTile, boardGraphicElements.outerUpperEdgeDelimeter);
    initializeFrameRow((boardRepresentationRowsCount - 1), boardGraphicElements.outerUpperLowerEdgeTile, boardGraphicElements.outerLowerEdgeDelimeter);
    for (int row = 3; row < (boardRepresentationRowsCount - 1); ++row)
    {
        bool oddRow = (row % 2) ? true : false;
        int innerTile = oddRow ? SYMBOL_SPACE : boardGraphicElements.innerHorizontalDelimeter;
        int delimeter = oddRow ? boardGraphicElements.innerVerticalDelimeter : boardGraphicElements.innerCross;
        initializeFrameRow(row, innerTile, delimeter);
    }
};

void createAndInitializeBoardRepresentation()
{
    cellRepresentationLength = isCompactMode ? cellRepresentationLengthCompactMode : cellRepresentationLengthExtendedMode;
    cellRepresentationBufferLength = cellRepresentationLength + 1;
    cellRepresentationBuffer = (char *)calloc(cellRepresentationBufferLength, sizeof(char));
    cellRepresentationBuffer[0] = '\0';

    distanceBetweenPLayersGameBoardAndDelemeter = 2 * cellRepresentationLength;
    distanceBetweenGameBoards = (2 * distanceBetweenPLayersGameBoardAndDelemeter + 1);

    gameBoardRepresentationCellCount = (gameBoardSize + 1);
    boardRepresentationRowsCount = (2 * gameBoardRepresentationCellCount + 1);
    boardRepresentationRowLength = (cellRepresentationLength * gameBoardRepresentationCellCount * 2 + distanceBetweenGameBoards + 1);

    int playerCaptionLength = strlen(playerCaptionBase);
    if (isGodMode)
    {
        playerCaptionLength += strlen(playerCaptionGodeModeSuffix);
    }
    ++playerCaptionLength;
    playerCaption = (char *)calloc(playerCaptionLength, sizeof(char));
    strcat_s(playerCaption, playerCaptionLength, playerCaptionBase);
    if (isGodMode)
    {
        strcat_s(playerCaption, playerCaptionLength, playerCaptionGodeModeSuffix);
    }

    initializeBoardGraphicElements();
    initializeCellPaterns();

    crossBoardSeparatorPosition = (cellRepresentationLength * gameBoardRepresentationCellCount + distanceBetweenPLayersGameBoardAndDelemeter);

    upperFrame = (char *)calloc(boardRepresentationRowLength, sizeof(char));
    memset(upperFrame, boardGraphicElements.innerHorizontalDelimeter, boardRepresentationRowLength);
    upperFrame[boardRepresentationRowLength - 1] = '\0';

    lowerFrame = (char *)calloc(boardRepresentationRowLength, sizeof(char));
    memset(lowerFrame, boardGraphicElements.innerHorizontalDelimeter, boardRepresentationRowLength);
    lowerFrame[boardRepresentationRowLength - 1] = '\0';

    boardRepresentation = (char**)calloc(boardRepresentationRowsCount, sizeof(char*));
    for (int index = 0; index < boardRepresentationRowsCount; ++index)
    {
        boardRepresentation[index] = (char *)calloc(boardRepresentationRowLength, sizeof(char));
        boardRepresentation[index] = (char *)memset(boardRepresentation[index], ' ', (boardRepresentationRowLength - 1));
        boardRepresentation[index][crossBoardSeparatorPosition] = boardGraphicElements.innerVerticalDelimeter;
        boardRepresentation[index][boardRepresentationRowLength - 1] = '\0';
    }
    memcpy(&boardRepresentation[0][getRowPosition(0, true)], playerCaption, strlen(playerCaption));
    memcpy(&boardRepresentation[0][getRowPosition(0, false)], computerCaption, strlen(computerCaption));

    initializeBoardRepresentationFrame();
};

char getTileSymbol(GameBoardFieldTypes inFiledType)
{
    char result = boardGraphicElements.emptyField;
    switch (inFiledType)
    {
    case FIELD_TYPE_UNAVAILABLE:
        result = boardGraphicElements.unavailableField;
        break;
    case FIELD_TYPE_HIT:
        result = boardGraphicElements.hitField;
        break;
    case FILED_TYPE_SHIP_NOT_FINISHED:
        result = boardGraphicElements.shipNotFinished;
        break;
    case FIELD_TYPE_SHIP_DAMAGED:
        result = boardGraphicElements.shipDamagedField;
        break;
    case FIELD_TYPE_SHIP_DESTROYED:
        result = boardGraphicElements.shipDestroyedField;
        break;
    case FIELD_TYPE_SHIP_NORMAL:
        result = boardGraphicElements.shipNormalField;
        break;
    case FIELD_TYPE_EMPTY:
    default:
        result = boardGraphicElements.emptyField;
        break;
    }
    return result;
}

void fillFrameWithGameBoardData()
{
    for (int row = 0; row < gameBoardSize; ++row)
    {
        for (int cell = 0; cell < gameBoardSize; ++cell)
        {
            char tileSymbol = getTileSymbol(playerGameBoard[row][cell].fieldType);
            setFieldTypeCellRepresentationInstance(row, cell, true, tileSymbol);

            GameBoardFieldTypes computerFieldType = isGodMode
                ? computerGameBoard[row][cell].fieldType
                : ((computerGameBoard[row][cell].fieldType == FIELD_TYPE_SHIP_NORMAL)
                || (computerGameBoard[row][cell].fieldType == FIELD_TYPE_UNAVAILABLE))
                ? FIELD_TYPE_EMPTY
                : computerGameBoard[row][cell].fieldType;
            tileSymbol = getTileSymbol(computerFieldType);
            setFieldTypeCellRepresentationInstance(row, cell, false, tileSymbol);
        }
    }
};

void showLegend()
{
    printf("\n");
    printf(legendCaption);
    printf(legendEmptyField, boardGraphicElements.emptyField);
    printf(legendHitField, boardGraphicElements.hitField);
    printf(legendFieldUnavailable, boardGraphicElements.unavailableField);
    printf(legendShipNotFinished, boardGraphicElements.shipNotFinished);
    printf(legendShipNormal, boardGraphicElements.shipNormalField);
    printf(legendShipDamanged, boardGraphicElements.shipDamagedField);
    printf(legendShipDestroyed, boardGraphicElements.shipDestroyedField);
    printf("\n");
}

void showBoards()
{
    fillFrameWithGameBoardData();
    printf("\n%s\n", upperFrame);
    for (int row = 0; row < boardRepresentationRowsCount; ++row)
    {
        printf("%s\n", boardRepresentation[row]);
    }
    printf("%s\n", lowerFrame);
    printf("\n");
};

void destroyBoardRepresentation()
{
    free(cellRepresentationBuffer);
    free(playerCaption);
    free(upperFrame);
    free(lowerFrame);
    for (int row = 0; row < boardRepresentationRowsCount; ++row)
    {
        free(boardRepresentation[row]);
    }
    free(boardRepresentation);
};

void initializeGameBoardShipCollection(GameBoardShip * inOutShipCollection)
{
    int index = 0;
    for (int shipLength = SHIP_LENGTH_1_LINE; shipLength <= SHIP_LENGTH_4_LINE; ++shipLength)
    {
        for (int shipsAmount = shipLength; shipsAmount <= 4; ++shipsAmount)
        {
            inOutShipCollection[index].isDestroyed = false;
            inOutShipCollection[index].shipLength = (GameBaordShipLengths)shipLength;
            inOutShipCollection[index].coordinates = (GameBoardShipTileCoordinates *)calloc(shipLength, sizeof(GameBoardShipTileCoordinates));
            ++index;
        }
    }
}

void createAndInitializeGameBoardFields(GameBoardField ** inGameBoardField)
{
    for (int row = 0; row < gameBoardSize; ++row)
    {
        inGameBoardField[row] = (GameBoardField *)calloc(gameBoardSize, sizeof(GameBoardField));
        for (int column = 0; column < gameBoardSize; ++column)
        {
            inGameBoardField[row][column].fieldType = FIELD_TYPE_EMPTY;
            inGameBoardField[row][column].gameBoardShip = nullptr;
        }
    }
};

void resetUsedCoordinates()
{
    for (int index = 0; index < possibleCoordinatesCount; ++index)
    {
        usedCoordinates[index] = false;
    }
};

void createAndInitializeGameBoards()
{
    possibleCoordinatesCount = gameBoardSize * gameBoardSize;
    usedCoordinates = (bool *)calloc(possibleCoordinatesCount, sizeof(bool));
    resetUsedCoordinates();

    rowCoordinates = (char *)calloc(gameBoardSize, sizeof(char));
    for (int row = 0; row < gameBoardSize; ++row)
    {
        rowCoordinates[row] = letterOfFirstRow + row;
    }

    playerGameBoard = (GameBoardField **)calloc(gameBoardSize, sizeof(GameBoardField *));
    createAndInitializeGameBoardFields(playerGameBoard);

    playerGameBoardShips = (GameBoardShip *)calloc(gameBoardShipsCount, sizeof(GameBoardShip));
    initializeGameBoardShipCollection(playerGameBoardShips);

    computerGameBoard = (GameBoardField **)calloc(gameBoardSize, sizeof(GameBoardField *));
    createAndInitializeGameBoardFields(computerGameBoard);

    computerGameBoardShips = (GameBoardShip *)calloc(gameBoardShipsCount, sizeof(GameBoardShip));
    initializeGameBoardShipCollection(computerGameBoardShips);

    srand((unsigned int)time(nullptr));
};

void destroyGameBoards()
{
    free(rowCoordinates);

    for (int row = 0; row < gameBoardSize; ++row)
    {
        free(playerGameBoard[row]);
        free(computerGameBoard[row]);
    }

    for (int index = 0; index < gameBoardShipsCount; ++index)
    {
        free(playerGameBoardShips[index].coordinates);
        free(computerGameBoardShips[index].coordinates);
    }

    free(playerGameBoardShips);
    free(computerGameBoardShips);

    free(playerGameBoard);
    free(computerGameBoard);
};

void setRandomNotUsedCoordinates()
{
    int amountOfCoordinatesLeft = 0;
    for (int index = 0; index < possibleCoordinatesCount; ++index)
    {
        if (!usedCoordinates[index])
        {
            ++amountOfCoordinatesLeft;
        }
    }
    int coordinates = 0;
    int availableIndex = (amountOfCoordinatesLeft != 0) ? (rand() % amountOfCoordinatesLeft) + 1 : 0;
    for (int index = 0; index < possibleCoordinatesCount; ++index)
    {
        if (!usedCoordinates[index])
        {
            ++coordinates;
        }
        if (coordinates == availableIndex)
        {
            coordinates = index;
            break;
        }
    }

    usedCoordinates[coordinates] = true;
    randomCoordinates.row = coordinates / gameBoardSize;
    randomCoordinates.cell = coordinates % gameBoardSize;
};

bool isNotCoordinateInRange(int inCoordinate)
{
    return ((inCoordinate < 0) || (inCoordinate >= gameBoardSize));
}

bool isCoordinateInRange(int inCoordinate)
{
    return ((inCoordinate >= 0) && (inCoordinate < gameBoardSize));
}

void surroundShipTile(GameBoardField ** inGameBoardFields, int inRow, int inCell, GameBoardFieldTypes inFieldType, bool inDoSetUsedCoordinates)
{
    for (int row = -1; row <= 1; ++row)
    {
        for (int cell = -1; cell <= 1; ++cell)
        {
            if (isNotCoordinateInRange(inRow + row) || isNotCoordinateInRange(inCell + cell))
            {
                continue;
            }
            usedCoordinates[row * gameBoardSize + cell] = true;
            if ((inGameBoardFields[inRow + row][inCell + cell].fieldType == FIELD_TYPE_EMPTY)
                || (inGameBoardFields[inRow + row][inCell + cell].fieldType == FIELD_TYPE_UNAVAILABLE))
            {
                inGameBoardFields[inRow + row][inCell + cell].fieldType = inFieldType;
                usedCoordinates[((inRow + row)*gameBoardSize + (inCell + cell))] = true;
            }
        }
    }
}

bool isCellAvailableForPlacement(GameBoardField ** inGameBoardFields, int inRow, int inCell)
{
    bool result = true;

    for (int row = -1; row <= 1; ++row)
    {
        for (int cell = -1; cell <= 1; ++cell)
        {
            if (isNotCoordinateInRange(inRow + row) || isNotCoordinateInRange(inCell + cell))
            {
                continue;
            }
            result &= ((inGameBoardFields[inRow + row][inCell + cell].fieldType == FIELD_TYPE_EMPTY)
                || (inGameBoardFields[inRow + row][inCell + cell].fieldType == FIELD_TYPE_UNAVAILABLE)
                || (inGameBoardFields[inRow + row][inCell + cell].fieldType == FILED_TYPE_SHIP_NOT_FINISHED));
        }
    }

    return result;

    //bool result = (inGameBoardFields[inRow + 1][inCell].fieldType == FIELD_TYPE_EMPTY);
    //result &= ((inGameBoardFields[inRow + 1][inCell].fieldType == FIELD_TYPE_EMPTY)
    //    || ((inGameBoardFields[inRow + 1][inCell].fieldType == FIELD_TYPE_UNAVAILABLE)));
    //result &= ((inGameBoardFields[inRow - 1][inCell].fieldType == FIELD_TYPE_EMPTY)
    //    || ((inGameBoardFields[inRow - 1][inCell].fieldType == FIELD_TYPE_UNAVAILABLE)));
    //result &= ((inGameBoardFields[inRow][inCell + 1].fieldType == FIELD_TYPE_EMPTY)
    //    || ((inGameBoardFields[inRow][inCell + 1].fieldType == FIELD_TYPE_UNAVAILABLE)));
    //result &= ((inGameBoardFields[inRow][inCell - 1].fieldType == FIELD_TYPE_EMPTY)
    //    || ((inGameBoardFields[inRow][inCell - 1].fieldType == FIELD_TYPE_UNAVAILABLE)));
    //result &= ((inGameBoardFields[inRow + 1][inCell + 1].fieldType == FIELD_TYPE_EMPTY)
    //    || ((inGameBoardFields[inRow + 1][inCell + 1].fieldType == FIELD_TYPE_UNAVAILABLE)));
    //result &= ((inGameBoardFields[inRow + 1][inCell - 1].fieldType == FIELD_TYPE_EMPTY)
    //    || ((inGameBoardFields[inRow + 1][inCell - 1].fieldType == FIELD_TYPE_UNAVAILABLE)));
    //result &= ((inGameBoardFields[inRow - 1][inCell + 1].fieldType == FIELD_TYPE_EMPTY)
    //    || ((inGameBoardFields[inRow - 1][inCell + 1].fieldType == FIELD_TYPE_UNAVAILABLE)));
    //result &= ((inGameBoardFields[inRow - 1][inCell - 1].fieldType == FIELD_TYPE_EMPTY)
    //    || ((inGameBoardFields[inRow - 1][inCell - 1].fieldType == FIELD_TYPE_UNAVAILABLE)));
};

void resetCoordinates(GameBoardShipTileCoordinates * inCoordinates, int inCoordinatesCount)
{
    for (int index = 0; index < inCoordinatesCount; ++index)
    {
        inCoordinates[index].row = -1;
        inCoordinates[index].cell = -1;
    }
}

int getMax(int inLeft, int inRight)
{
    return (inLeft < inRight) ? inRight : inLeft;
};

int getMin(int inLeft, int inRight)
{
    return (inLeft > inRight) ? inRight : inLeft;
};

int spaceLookup(GameBoardField ** inGameBoardFields, GameBoardShipTileCoordinates & inOutLookupStart, GameBoardShipTileCoordinates & inOutLookupEnd)
{
    int resultSpace = 0;
    GameBoardShipTileCoordinates lookupResultStart;
    lookupResultStart.row = -1;
    lookupResultStart.cell = -1;
    GameBoardShipTileCoordinates lookupResultEnd;
    lookupResultEnd.row = -1;
    lookupResultEnd.cell = -1;
    for (int row = inOutLookupStart.row; row <= inOutLookupEnd.row; ++row)
    {
        for (int cell = inOutLookupStart.cell; cell <= inOutLookupEnd.cell; ++cell)
        {
            if (isCellAvailableForPlacement(inGameBoardFields, row, cell))
            {
                if (resultSpace == 0)
                {
                    lookupResultStart.row = row;
                    lookupResultStart.cell = cell;
                }
                lookupResultEnd.row = row;
                lookupResultEnd.cell = cell;
                ++resultSpace;
            }
        }
    }
    inOutLookupStart = lookupResultStart;
    inOutLookupEnd = lookupResultEnd;
    return resultSpace;
};

void automaticPlacement(GameBoardField ** inGameBoardFields, GameBoardShip * inGameBoardShips)
{
    resetUsedCoordinates();
    for (int shipIndex = 0; shipIndex < gameBoardShipsCount; ++shipIndex)
    {
        GameBoardShip * ship = &inGameBoardShips[shipIndex];
        bool allSet = false;
        while (!allSet)
        {
            setRandomNotUsedCoordinates();

            GameBoardShipTileCoordinates horizontalLookupResultStart;
            horizontalLookupResultStart.row = randomCoordinates.row;
            horizontalLookupResultStart.cell = getMax(0, (randomCoordinates.cell - ship->shipLength + 1));

            GameBoardShipTileCoordinates horizontalLookupResultEnd;
            horizontalLookupResultEnd.row = randomCoordinates.row;
            horizontalLookupResultEnd.cell = getMin((gameBoardSize - 1), (randomCoordinates.cell + ship->shipLength - 1));

            int horizontalSpace = spaceLookup(inGameBoardFields, horizontalLookupResultStart, horizontalLookupResultEnd);


            GameBoardShipTileCoordinates verticalLookupResultStart;
            verticalLookupResultStart.row = getMax(0, (randomCoordinates.row - ship->shipLength + 1));
            verticalLookupResultStart.cell = randomCoordinates.cell;

            GameBoardShipTileCoordinates verticalLookupResultEnd;
            verticalLookupResultEnd.row = getMin((gameBoardSize - 1), (randomCoordinates.row + ship->shipLength - 1));
            verticalLookupResultEnd.cell = randomCoordinates.cell;

            int verticalSpace = spaceLookup(inGameBoardFields, verticalLookupResultStart, verticalLookupResultEnd);

            if ((verticalSpace < ship->shipLength) && (horizontalSpace < ship->shipLength))
            {
                continue;
            }

            bool isStart = (rand() % 2) ? true : false;
            bool isVertical = true;
            if ((verticalSpace >= ship->shipLength) && (horizontalSpace >= ship->shipLength))
            {
                bool isVertical = (rand() % 2) ? true : false;
            }
            else if (horizontalSpace >= ship->shipLength)
            {
                isVertical = false;
            }

            GameBoardShipTileCoordinates start;
            GameBoardShipTileCoordinates end;

            if (isVertical)
            {
                start.row = isStart ? verticalLookupResultStart.row : (verticalLookupResultEnd.row - ship->shipLength + 1);
                start.cell = randomCoordinates.cell;
                end.row = isStart ? (verticalLookupResultStart.row + ship->shipLength - 1) : verticalLookupResultEnd.row;
                end.cell = randomCoordinates.cell;
            }
            else
            {
                start.row = randomCoordinates.row;
                start.cell = isStart ? horizontalLookupResultStart.cell : (horizontalLookupResultEnd.cell - ship->shipLength + 1);
                end.row = randomCoordinates.row;
                end.cell = isStart ? (horizontalLookupResultStart.cell + ship->shipLength - 1) : horizontalLookupResultEnd.cell;
            }

            int coordinatesIndex = 0;
            for (int row = start.row; row <= end.row; ++row)
            {
                for (int cell = start.cell; cell <= end.cell; ++cell)
                {
                    inGameBoardFields[row][cell].fieldType = FIELD_TYPE_SHIP_NORMAL;
                    inGameBoardFields[row][cell].gameBoardShip = ship;
                    surroundShipTile(inGameBoardFields, row, cell, FIELD_TYPE_UNAVAILABLE, false);
                    ship->coordinates[coordinatesIndex].row = row;
                    ship->coordinates[coordinatesIndex].cell = cell;
                    ++coordinatesIndex;
                }
            }

            allSet = true;
        }
    }
}

bool isProperShip(GameBoardShip inShip)
{
    if (inShip.shipLength == SHIP_LENGTH_1_LINE)
    {
        return true;
    }

    for (int coordinateIndex = 0; coordinateIndex < (inShip.shipLength - 1); ++coordinateIndex)
    {
        int rowSpan = 0;
        int cellSpan = 0;
        GameBoardShipTileCoordinates current = inShip.coordinates[coordinateIndex];
        for (int checkCoordinateIndex = (coordinateIndex + 1); checkCoordinateIndex < inShip.shipLength; ++checkCoordinateIndex)
        {
            int currentRowSpan = abs(inShip.coordinates[checkCoordinateIndex].row - current.row);
            int currentCellSpan = abs(inShip.coordinates[checkCoordinateIndex].cell - current.cell);
            if ((currentRowSpan > 0) && (currentCellSpan > 0))
            {
                return false;
            }
            if ((currentRowSpan == 0) && (currentCellSpan >= inShip.shipLength))
            {
                return false;
            }
            if ((currentCellSpan == 0) && (currentRowSpan >= inShip.shipLength))
            {
                return false;
            }
            rowSpan += currentRowSpan;
            cellSpan += currentCellSpan;
        }
        if ((rowSpan > 0) && (cellSpan > 0))
        {
            return false;
        }
    }
    return true;
}

void printLastEnteredCoordinates(int inRow, int inCell)
{
    printf("\nLast entered coordinates: [%c,%1d] or '%c%1d'", rowCoordinates[inRow], inCell, rowCoordinates[inRow], inCell);
}

void manualPlacement()
{
    for (int shipIndex = 0; shipIndex < gameBoardShipsCount; ++shipIndex)
    {
        GameBoardShip ship = playerGameBoardShips[shipIndex];
        GameBoardFieldTypes * previousFieldTypes = (GameBoardFieldTypes *)calloc(ship.shipLength, sizeof(GameBoardFieldTypes));
        printf("\nPlacing ship with %1d tile(s).", ship.shipLength);
        for (int tileIndex = 0; tileIndex < ship.shipLength; ++tileIndex)
        {
            GameBoardShipTileCoordinates newTileCoordinates = lineCoordinatesRequest();
            int newTileRow = newTileCoordinates.row;
            int newTileCell = newTileCoordinates.cell;
            if (!isCellAvailableForPlacement(playerGameBoard, newTileRow, newTileCell))
            {
                printf("\nEntered cell is not avaialbe for placement.\nPlace tile in the way it doesn't touch other ships nor by side nor by corner.");
                --tileIndex;
                continue;
            }
            previousFieldTypes[tileIndex] = playerGameBoard[newTileRow][newTileCell].fieldType;
            playerGameBoard[newTileRow][newTileCell].fieldType = FILED_TYPE_SHIP_NOT_FINISHED;
            showBoards();
            printLastEnteredCoordinates(newTileRow, newTileCell);
            if (lineYesOrNoRequest(shipTilePlacementConfirmation))
            {
                ship.coordinates[tileIndex].row = newTileRow;
                ship.coordinates[tileIndex].cell = newTileCell;
            }
            else
            {
                playerGameBoard[newTileRow][newTileCell].fieldType = previousFieldTypes[tileIndex];
                --tileIndex;
            }
        }
        if (lineYesOrNoRequest(shipPlacementConfirmation))
        {
            if (isProperShip(ship))
            {
                for (int coordinateIndex = 0; coordinateIndex < ship.shipLength; ++coordinateIndex)
                {
                    GameBoardShipTileCoordinates coordinates = ship.coordinates[coordinateIndex];
                    playerGameBoard[coordinates.row][coordinates.cell].fieldType = FIELD_TYPE_SHIP_NORMAL;
                    playerGameBoard[coordinates.row][coordinates.cell].gameBoardShip = &ship;
                    surroundShipTile(playerGameBoard, coordinates.row, coordinates.cell, FIELD_TYPE_UNAVAILABLE, false);
                }
            }
            else
            {
                printf("\nWrong ship configuration. Tiles must be placed near other ship tiles and form a line. Please try again.");
                for (int tileIndex = 0; tileIndex < ship.shipLength; ++tileIndex)
                {
                    GameBoardShipTileCoordinates coordinates = ship.coordinates[tileIndex];
                    playerGameBoard[coordinates.row][coordinates.cell].fieldType = previousFieldTypes[tileIndex];
                }
                --shipIndex;
            }
        }
        free(previousFieldTypes);
        showBoards();
    }
    printf("\nShip placement complete.");
};

bool doMove()
{
    playerDestroyedShipsCount = 0;
    computerDestroyedShipsCount = 0;
    for (int index = 0; index < gameBoardShipsCount; ++index)
    {
        if (playerGameBoardShips[index].isDestroyed)
        {
            ++playerDestroyedShipsCount;
        }
        if (computerGameBoardShips[index].isDestroyed)
        {
            ++computerDestroyedShipsCount;
        }
    }

    return ((playerDestroyedShipsCount < gameBoardShipsCount) && (computerDestroyedShipsCount < gameBoardShipsCount));
}

void doHitShip(GameBoardField ** inGameBoard, int inRow, int inCell, bool inDoSetUsedCoordinates)
{
    GameBoardShip * ship = inGameBoard[inRow][inCell].gameBoardShip;
    int damagedTilesCount = 0;
    for (int coordinatesIndex = 0; coordinatesIndex < ship->shipLength; ++coordinatesIndex)
    {
        int currentRow = ship->coordinates[coordinatesIndex].row;
        int currentCell = ship->coordinates[coordinatesIndex].cell;
        if (inGameBoard[currentRow][currentCell].fieldType == FIELD_TYPE_SHIP_DAMAGED)
        {
            ++damagedTilesCount;
        }
    }
    if (damagedTilesCount == ship->shipLength)
    {
        ship->isDestroyed = true;
        for (int coordinatesIndex = 0; coordinatesIndex < ship->shipLength; ++coordinatesIndex)
        {
            int currentRow = ship->coordinates[coordinatesIndex].row;
            int currentCell = ship->coordinates[coordinatesIndex].cell;
            inGameBoard[currentRow][currentCell].fieldType = FIELD_TYPE_SHIP_DESTROYED;
            surroundShipTile(inGameBoard, currentRow, currentCell, FIELD_TYPE_HIT, inDoSetUsedCoordinates);
        }
    }
}

void resetCurrentTileHit()
{
    for (int index = 0; index < maximumAmountOfShootingPointInAgenda; ++index)
    {
        currentTilesHit[index].row = -1;
        currentTilesHit[index].cell = -1;
    }
}

void resetAgenda()
{
    for (int index = 0; index < maximumAmountOfShootingPointInAgenda; ++index)
    {
        shootingAgenda[index].row = -1;
        shootingAgenda[index].cell = -1;
    }
}

bool isValidShootingPoint(int inRow, int inCell)
{
    if ((inRow < 0) || (inCell < 0) || (inRow >= gameBoardSize) || (inCell >= gameBoardSize))
    {
        return false;
    }

    if (usedCoordinates[inRow * gameBoardSize + inCell])
    {
        return false;
    }

    GameBoardFieldTypes result = FIELD_TYPE_EMPTY;
    switch (playerGameBoard[inRow][inCell].fieldType)
    {
    case FIELD_TYPE_HIT:
    case FIELD_TYPE_SHIP_DAMAGED:
    case FIELD_TYPE_SHIP_DESTROYED:
        result = FIELD_TYPE_HIT;
        break;
    case FIELD_TYPE_EMPTY:
    case FIELD_TYPE_UNAVAILABLE:
    case FIELD_TYPE_SHIP_NORMAL:
    default:
        result = FIELD_TYPE_EMPTY;
        break;
    }

    return (result == FIELD_TYPE_EMPTY);
}

void setShootingAgenda(int inRow, int inCell, int & inOutIndex)
{
    if (isValidShootingPoint(inRow, inCell))
    {
        shootingAgenda[inOutIndex].row = inRow;
        shootingAgenda[inOutIndex].cell = inCell;
        ++inOutIndex;
    }
}

void formShootingAgenda(int inRow, int inCell)
{
    resetAgenda();
    int amountOfTilesHit = 0;

    int minimalRow = inRow;
    int maximalRow = inRow;

    int minimalCell = inCell;
    int maximalCell = inCell;

    for (int index = 0; index < maximumAmountOfShootingPointInAgenda; ++index)
    {
        if ((currentTilesHit[index].row == -1) && (currentTilesHit[index].cell == -1))
        {
            currentTilesHit[index].row = inRow;
            currentTilesHit[index].cell = inCell;
            ++amountOfTilesHit;
            break;
        }
        else
        {
            minimalRow = (currentTilesHit[index].row < minimalRow) ? currentTilesHit[index].row : minimalRow;
            maximalRow = (currentTilesHit[index].row > maximalRow) ? currentTilesHit[index].row : maximalRow;
            minimalCell = (currentTilesHit[index].cell < minimalCell) ? currentTilesHit[index].cell : minimalCell;
            maximalCell = (currentTilesHit[index].cell > maximalCell) ? currentTilesHit[index].cell : maximalCell;
        }
    }

    int setNextTargetCount = 0;

    bool shootVertically = (minimalCell == maximalCell);
    bool shootHorizontally = (minimalRow == maximalRow);

    if (shootHorizontally)
    {
        setShootingAgenda(inRow, (minimalCell - 1), setNextTargetCount);
        setShootingAgenda(inRow, (maximalCell + 1), setNextTargetCount);
    }

    if (shootVertically)
    {
        setShootingAgenda((minimalRow - 1), inCell, setNextTargetCount);
        setShootingAgenda((maximalRow + 1), inCell, setNextTargetCount);
    }
}

void main()
{
    isCompactMode = lineYesOrNoRequest(compactModeSelectionRequest);
    isPortableSymbolSetUsed = lineYesOrNoRequest(symbolSetSelectionRequestPrompt);
    isAutomaticPlacement = lineYesOrNoRequest(shipPlacementModeSelectionRequestPrompt);
    isGodMode = lineYesOrNoRequest(godModeSelectionRequest);

    createAndInitializeGameBoards();
    createAndInitializeBoardRepresentation();

    showLegend();

    if (isAutomaticPlacement)
    {
        automaticPlacement(playerGameBoard, playerGameBoardShips);
    }
    else
    {
        showBoards();
        manualPlacement();
    }
    automaticPlacement(computerGameBoard, computerGameBoardShips);

    showBoards();

    int amountOfMoves = 0;
    int startTime = (int)time(nullptr);
    bool playerMove = rand() % 2 ? true : false;
    resetUsedCoordinates();
    resetAgenda();
    resetCurrentTileHit();
    while (doMove())
    {
        if (playerMove)
        {
            printf("\nPlayer shoots.");
            ++amountOfMoves;
            GameBoardShipTileCoordinates newTileCoordinates = lineCoordinatesRequest();
            int row = newTileCoordinates.row;
            int cell = newTileCoordinates.cell;

            GameBoardField * computerField = &computerGameBoard[row][cell];

            bool shooted = false;

            switch (computerField->fieldType)
            {
            case FIELD_TYPE_HIT:
            case FIELD_TYPE_SHIP_DAMAGED:
            case FIELD_TYPE_SHIP_DESTROYED:
                printf("\nYou already hit that field. Please try again.");
                ++amountOfMoves;
                break;
            case FIELD_TYPE_EMPTY:
            case FIELD_TYPE_UNAVAILABLE:
                computerField->fieldType = FIELD_TYPE_HIT;
                playerMove = !playerMove;
                shooted = true;
                break;
            case FIELD_TYPE_SHIP_NORMAL:
                computerField->fieldType = FIELD_TYPE_SHIP_DAMAGED;
                doHitShip(computerGameBoard, row, cell, false);
                shooted = true;
                break;
            default:
                printf("\nPlease try again.");
                ++amountOfMoves;
                break;
            }
            if (shooted)
            {
                showBoards();
            }
        }
        else
        {
            GameBoardShipTileCoordinates currentCoordinates;
            currentCoordinates.row = -1;
            currentCoordinates.cell = -1;
            for (int agendaIndex = 0; agendaIndex < maximumAmountOfShootingPointInAgenda; ++agendaIndex)
            {
                if ((shootingAgenda[agendaIndex].row != -1) && (shootingAgenda[agendaIndex].cell != -1))
                {
                    currentCoordinates = shootingAgenda[agendaIndex];
                    shootingAgenda[agendaIndex].row = -1;
                    shootingAgenda[agendaIndex].cell = -1;
                    usedCoordinates[currentCoordinates.row*gameBoardSize + currentCoordinates.cell] = true;
                    break;
                }
            }
            if ((currentCoordinates.row == -1) && (currentCoordinates.cell == -1))
            {
                setRandomNotUsedCoordinates();
                currentCoordinates = randomCoordinates;
            }

            int currentRow = currentCoordinates.row;
            int currentCell = currentCoordinates.cell;

            GameBoardField * playerField = &playerGameBoard[currentRow][currentCell];

            bool shooted = false;

            switch (playerField->fieldType)
            {
            case FIELD_TYPE_HIT:
            case FIELD_TYPE_SHIP_DAMAGED:
            case FIELD_TYPE_SHIP_DESTROYED:
                usedCoordinates[currentRow*gameBoardSize + currentCell] = true;
                break;
            case FIELD_TYPE_EMPTY:
            case FIELD_TYPE_UNAVAILABLE:
                playerField->fieldType = FIELD_TYPE_HIT;
                playerMove = !playerMove;
                shooted = true;
                break;
            case FIELD_TYPE_SHIP_NORMAL:
                playerField->fieldType = FIELD_TYPE_SHIP_DAMAGED;
                doHitShip(playerGameBoard, currentRow, currentCell, true);
                if (playerGameBoard[currentRow][currentCell].gameBoardShip->isDestroyed)
                {
                    resetAgenda();
                    resetCurrentTileHit();
                }
                else
                {
                    formShootingAgenda(currentRow, currentCell);
                }
                shooted = true;
                break;
            default:
                break;
            }

            if (shooted)
            {
                printf("\nComputer shoots at [%c, %d].", rowCoordinates[currentRow], currentCell);
                showBoards();
            }
        }
    }

    printf("\nAnd the winner is: %s.", (playerDestroyedShipsCount > computerDestroyedShipsCount) ? computerCaption : playerCaption);

    printf("\n");

    destroyBoardRepresentation();
    destroyGameBoards();
}
