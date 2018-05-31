<?php

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/

Route::get('/', function () {return view('index');});
Route::get('/varer', 'VareController@ListAll');
Route::get('/vare/ny', function () {return view('register');});
Route::post('/vare/ny', 'VareController@new');
Route::get('/vare/registrert', function () {return view('register');});
Route::get('/bestilt/{kunde}', 'HandlekurvController@BestillingOK');
Route::get('/handlekurv', 'HandlekurvController@ListAll');
Route::get('/handlekurv/{kunde}/{upc}', 'HandlekurvController@LeggTil');
Route::post('/handlekurv', 'HandlekurvController@LeggTilPost');