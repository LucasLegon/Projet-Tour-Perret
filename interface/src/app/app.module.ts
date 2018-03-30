import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms'; // <-- NgModel lives here
import { AppComponent } from './app.component';
import { HomeComponent } from './home/home.component';
import { EditorComponent } from './editor/editor.component';
import { AppRoutingModule } from './app-routing.module';
import { Tuto1Component } from './tuto/tuto1/tuto1.component';
import { Tuto2Component } from './tuto/tuto2/tuto2.component';
import { Tuto3Component } from './tuto/tuto3/tuto3.component';
import { AproposComponent } from './apropos/apropos.component';
import {NgbModule} from '@ng-bootstrap/ng-bootstrap';
import { TutoComponent } from './tuto/tuto.component';


@NgModule({
  declarations: [
    AppComponent,
    HomeComponent,
    Tuto1Component,
    EditorComponent,
    Tuto2Component,
    Tuto3Component,
    AproposComponent,
    TutoComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    AppRoutingModule,
    NgbModule.forRoot()
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
