// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    TArray<FString> ValidWords = UBullCowCartridge::GetValidWords(Words);
    IsoGrams = GetValidWords(Words);
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{

    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Welcome to BullCow game!"));
    int32 RandNum = FMath::RandRange(0, IsoGrams.Num() - 1);

    HiddenWord = IsoGrams[RandNum];
    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word."), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives"), Lives);
    PrintLine(TEXT("Hidden work is %s"), *HiddenWord);
    PrintLine(TEXT("Enter guess..."));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again..."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("Correct!"));
        EndGame();
        return;
    }

    if (!IsIsoGram(Guess))
    {
        PrintLine("No repeating letters, guess again");
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("Incorrect. You have %i lives left"), Lives);
        PrintLine(TEXT("The word is %i characters long"), HiddenWord.Len());
        return;
    }

    --Lives;
    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("Game Over."));
        PrintLine(TEXT("The hidden word was %s."), *HiddenWord);
        EndGame();
        return;
    }
    // give hint if 2 lives left
    if (Lives == 2)
    {
        PrintLine(TEXT("Word starts with %s and ends with %s."), *HiddenWord.Mid(0, 1), *HiddenWord.Mid(HiddenWord.Len() - 1, HiddenWord.Len()));
    }
    
    FBullCowCount Score = GetBullCows(Guess);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
    PrintLine(TEXT("Guess again, you have %i lives left"), Lives);
}

bool UBullCowCartridge::IsIsoGram(const FString& Word) const
{
    for (int32 I = 0; I + 1 < Word.Len(); I++)
    {
       if (Word[I] == Word[I+1])
       {
           return false;
       } 
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;
    
    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsoGram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;
    for (int32 I = 0; I < Guess.Len(); I++)
    {
        if (Guess[I] == HiddenWord[I])
        {
            Count.Bulls++;
            continue; // start loop over from here
        }

        for (int32 J = 0; J < HiddenWord.Len(); J++)
        {
            if (Guess[I] == HiddenWord[J])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}