package cz.cvut.fit.tjv.realestates.controller.converter;

import cz.cvut.fit.tjv.realestates.application.PropertyService;
import cz.cvut.fit.tjv.realestates.controller.dto.CustomerDTO;
import cz.cvut.fit.tjv.realestates.domain.Customer;
import cz.cvut.fit.tjv.realestates.domain.Property;
import org.springframework.stereotype.Component;

@Component
public class CustomerDTOConverter implements DTOConverter<CustomerDTO, Customer> {
    private final PropertyService propertyService;

    public CustomerDTOConverter(PropertyService propertyService) {
        this.propertyService = propertyService;
    }

    @Override
    public CustomerDTO toDTO(Customer customer) {
        //Using java Streams to map the list of objects to a list of id's from list of properties
        return new CustomerDTO(customer.getEmail(), customer.getFirstName(), customer.getLastName(),
                customer.getBirthDate(), customer.getSubleases().stream().map(Property::getId).toList());
    }

    @Override
    public Customer toEntity(CustomerDTO customerDTO) {
        return new Customer(customerDTO.getEmail(), customerDTO.getFirstName(), customerDTO.getLastName(),
                customerDTO.getBirthDate(), propertyService.getEntitiesByIds(customerDTO.getIdSubleases()));
    }
}
