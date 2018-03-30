import {Component} from '@angular/core';
import {NgbCarouselConfig} from '@ng-bootstrap/ng-bootstrap';
import { Tuto1Component } from './tuto1/tuto1.component';
import { Tuto2Component } from './tuto2/tuto2.component';
import { Tuto3Component } from './tuto3/tuto3.component';

@Component({
  selector: 'app-tuto',
  templateUrl: './tuto.component.html',
  providers: [NgbCarouselConfig] // add NgbCarouselConfig to the component providers

})

export class TutoComponent {
  constructor(config: NgbCarouselConfig) {
    // customize default values of carousels used by this component tree
    config.interval = 20000; //Transition de 20 secondes
    config.wrap = false;
    config.keyboard = false;
  }
}
